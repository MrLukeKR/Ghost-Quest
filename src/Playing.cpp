#include "header.h"
#include "Playing.h"
#include "JPGImage.h"
#include "LKREngine.h"
#include <string>
#include "Enemy.h"
#include "Ghost.h"
#include "Reaper.h"
#include "Spider.h"
#include <fstream>
#include <iostream>

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

using namespace std;


Playing::Playing(LKREngine* pEngine, bool loadGame) 
	: Stage(pEngine), m_engine(pEngine)
{
	LoadMapData("game1.gqm");
	UpdateCurrentView();

	xMax = mapWidth - 1;
	xMin = 1;
	yMax = mapHeight - 1;
	yMin = 1;

	mapCharWidth = (1.0 / (double)mapWidth) * 240;
	mapCharHeight = (1.0 / (double)mapHeight) * 69;
	
	InitialiseObjects();

	if (loadGame)
		LoadGame();
}


Playing::~Playing()
{

}

void Playing::InitialiseObjects(){
	player = new Hero(m_engine, this, 50, 50, wPlayerPosX, wPlayerPosY);

	m_engine->StoreObjectInArray(player);

	GenerateEnemies(10);
	GenerateSpider();
	GenerateReaper();
}

void Playing::GenerateEnemies(int amount){
	int xPos, yPos;

	for (int i = 0; i < amount; i++){
		do {
			xPos = rand() % (xMax - xMin + 1) + xMin;
			yPos = rand() % (yMax - yMin + 1) + yMin;
		} while (GetMapValue(xPos, yPos) < 5 || IsCharacterAt(xPos, yPos));

		m_engine->StoreObjectInArray(new Ghost(m_engine, this, 50, 50, xPos, yPos, player));
	}
}

void Playing::GenerateReaper(){
	int xPos, yPos;

	do {
		xPos = rand() % (xMax - xMin + 1) + xMin;
		yPos = rand() % (yMax - yMin + 1) + yMin;
	} while (GetMapValue(xPos, yPos) < 5 || IsCharacterAt(xPos, yPos));

	m_engine->StoreObjectInArray(new Reaper(m_engine, this, 50, 50, xPos, yPos, player));
}

void Playing::GenerateSpider(){
	int xPos, yPos;

	do {
		xPos = rand() % (xMax - xMin + 1) + xMin;
		yPos = rand() % (yMax - yMin + 1) + yMin;
	} while (GetMapValue(xPos, yPos) < 5 || IsCharacterAt(xPos, yPos));

	m_engine->StoreObjectInArray(new Spider(m_engine, this, 50, 50, xPos, yPos, player));
}

void Playing::DrawStrings(){
	m_engine->CopyBackgroundPixels(0, 0, m_engine->GetScreenWidth(), 100); //Redraw black upper border
	
	Draw();
	
	if (loadFlash > 0){
		m_engine->DrawScreenString(315, 35, "LOADED GAME", 0xFF0000, NULL);
		loadFlash--;
		return;
	}

	if (saveFlash > 0){
		m_engine->DrawScreenString(315, 35, "SAVED GAME", 0x00FF00, NULL);
		saveFlash--;
		return;
	}

	if (IsPaused()){
		if (pauseFlash < 5)
			m_engine->DrawScreenString(315, 35, "PAUSED GAME", 0xFFFFFF, NULL);
		else if (pauseFlash >= 10)
			pauseFlash = 0;
		pauseFlash++;
		return;
	}

	char sRemainingPoints[16] = "REM: ";
	char sExperiencePoints[17] = "EXP: ";
	char sLevel[9] = "LVL: ";
	char sDefencePoints[14] = "DEF: ";
	char sAttackPoints[13] = "ATT: ";
	
	char temp[3];

	itoa(player->GetRemainingExperience(), temp,10);
	strcat(sRemainingPoints, temp);

	itoa(player->GetExperiencePoints(), temp, 10);
	strcat(sExperiencePoints, temp);

	itoa(player->GetLevel(), temp, 10);
	strcat(sLevel, temp);

	itoa(player->GetDefencePoints(), temp, 10);
	strcat(sDefencePoints, temp);

	itoa(player->GetAttackPoints(), temp, 10);
	strcat(sAttackPoints, temp);

	m_engine->DrawScreenString(525, 5, "H:", 0xFFFFFF, NULL);
	m_engine->DrawScreenString(525, 35, "M:", 0xFFFFFF, NULL);
	m_engine->DrawScreenString(300, 5, sRemainingPoints, 0xFFFFFF, NULL);
	m_engine->DrawScreenString(300, 35, sExperiencePoints, 0xFFFFFF, NULL);
	m_engine->DrawScreenString(300, 65, sLevel, 0xFFFFFF, NULL);
	m_engine->DrawScreenString(525, 65, sDefencePoints, 0xFFFFFF, NULL);
	m_engine->DrawScreenString(665, 65, sAttackPoints, 0xFFFFFF, NULL);
}

void Playing::SceneTransition(int direction, unsigned int amount){
	int xShift = 0, yShift = 0;

	switch (direction){
	case UP:
		if (GetCameraY() - amount < 0)
			amount = GetCameraY();
		yShift = -amount;
		break;
	case LEFT:
		if (GetCameraX() - amount < 0)
			amount = GetCameraX();
		xShift = -amount;
		break;
	case DOWN:
		if (GetCameraY() + amount > mapHeight - screenTileHeight)
			amount = mapHeight - screenTileHeight-GetCameraY();
		yShift = amount;
		break;
	case RIGHT:
		if (GetCameraX() + amount > mapWidth - screenTileWidth)
			amount = mapWidth - screenTileWidth -GetCameraX();
		xShift = amount;
		break;
	}

	if (cameraPosX + xShift < 0)
		cameraPosX = 0;
	else if (cameraPosX + xShift > mapWidth - screenTileWidth)
		cameraPosX = mapWidth-screenTileWidth;
	else
		cameraPosX += xShift;

	if (cameraPosY + yShift < 0)
		cameraPosY = 0;
	else if (cameraPosY + yShift > mapWidth - screenTileWidth)
		cameraPosY = mapWidth - screenTileWidth;
	else
		cameraPosY += yShift;

	UpdateCurrentView();
	LKRTileManager& m_tm = m_engine->GetTileManager();

	//UPDATE STAGE
	for (int x = 0; x < screenTileWidth; x ++)
		for (int y = 0; y < screenTileHeight; y++)
			m_tm.UpdateTile(m_engine, x,y,currentView[y][x]-'a');

	m_engine->Redraw(true);
}

void Playing::SaveGame(){ //Saves the player's stats to a file
	ofstream saveFile;
	saveFile.open("ghostQuestSave.gqs");
	saveFile << player->GetAttackPoints() << ",";
	saveFile << player->GetDefencePoints() << ",";
	saveFile << player->GetExperiencePoints() << ",";
	saveFile << player->GetRemainingExperience() << ",";
	saveFile << player->GetLevel() << ",";
	saveFile << player->GetHealth() << ",";
	saveFile << player->GetMagic() << ",";
	saveFile << player->GetMaxHealth() << ",";
	saveFile << player->GetMaxMagic() << ",";
	saveFile << player->GetWorldX() << ",";
	saveFile << player->GetWorldY() << ",";
	saveFile << player->GetLocalX() << ",";
	saveFile << player->GetLocalY() << ",";
	saveFile << cameraPosX << ",";
	saveFile << cameraPosY;
	saveFile.close();

	saveFlash = 10;
}

void Playing::LoadGame(){ //Loads player stats from a file
	ifstream saveFile;
	string line;
	int itemCount = 0;

	saveFile.open("ghostQuestSave.gqs",ios::in);

	if (!saveFile.is_open())
		return;

	while (getline(saveFile, line, ',')){
		switch (itemCount++){
		case 0: player->SetAttackPoints(stoi(line));		break;
		case 1: player->SetDefencePoints(stoi(line));		break;
		case 2: player->SetExperience(stoi(line));			break;
		case 3: player->SetRemainingExperience(stoi(line));	break;
		case 4: player->SetLevel(stoi(line));				break;
		case 5: player->SetHealth(stoi(line));				break;
		case 6: player->SetMagic(stoi(line));				break;
		case 7:	player->SetMaxHealth(stoi(line));			break;
		case 8:	player->SetMaxMagic(stoi(line));			break;
		case 9: player->SetWorldX(stoi(line));				break;
		case 10: player->SetWorldY(stoi(line));				break;
		case 11: player->SetLocalX(stoi(line));				break;
		case 12: player->SetLocalY(stoi(line));				break;
		case 13: cameraPosX = stoi(line);
				 player->SetCameraX(cameraPosX);			break;
		case 14: cameraPosY = stoi(line);
				 player->SetCameraY(cameraPosY);			break;
		}
	}

	saveFile.close();
	UpdateCurrentView();

	loadFlash = 10;
}

void Playing::ProcessInput(int button, int x, int y){
	player->ProcessInput(button, x, y);
}

void Playing::ProcessInput(int keyCode){
	if (keyCode == SDLK_q){
		SaveGame();
		m_engine->StageSwitch(6);
	}
	else if (keyCode == SDLK_ESCAPE){
		SaveGame();

	}
	else
	{
		player->ProcessInput(keyCode);

		UpdateCurrentView();
	}
}

void Playing::ProcessMouseMovement(int x, int y){
	player->ProcessMouseMovement(x, y);
}

void Playing::DoUpdate(int iCurrentTime){
	if (paused)
		return;
	

	if (frame++ % 100 == 0)
	GenerateEnemies(1);
}

void Playing::Draw(){
	if (IsPaused() || saveFlash > 0 || loadFlash > 0)
		return;
		
	char sHealth[9] = "";
	char sMagic[9] = "";
	char temp[4];

	int charPosX, charPosY;

	itoa(player->GetHealth(), temp, 10);
	strcat(sHealth, temp);
	strcat(sHealth, "/");
	itoa(player->GetMaxHealth(), temp, 10);
	strcat(sHealth, temp);

	itoa(player->GetMagic(), temp, 10);
	strcat(sMagic, temp);
	strcat(sMagic, "/");
	itoa(player->GetMaxMagic(), temp, 10);
	strcat(sMagic, temp);

	hWidth = 225 * player->GetHealthPercentage();
	mWidth = 225 * player->GetMagicPercentage();

	m_engine->CopyBackgroundPixels(hXStart, hYStart, 225, hYStart + (mYStart - hYStart) + 20);

	if (hWidth > 0)
		m_engine->DrawScreenRectangle(hXStart, hYStart, hXStart + hWidth - 1, hYStart + 20, 0xff0000);
	if (mWidth > 0)
		m_engine->DrawScreenRectangle(mXStart, mYStart, mXStart + mWidth - 1, mYStart + 20, 0x0000FF);

	m_engine->DrawScreenRectangle(10, 10, 260, 89, 0x444444);

	Enemy* currEnemy;

	for (int i = 1; i < m_engine->GetNumberOfObjects(); i++){
		currEnemy = (Enemy*)m_engine->GetDisplayableObject(i);
		charPosX = 10 + ((double)currEnemy->GetWorldX() / (double)mapWidth) * 250;
		charPosY = 10 + ((double)currEnemy->GetWorldY() / (double)mapHeight) * 79;
		if (currEnemy->GetLevel() > 2 * player->GetLevel())
			m_engine->DrawScreenRectangle(charPosX, charPosY, charPosX + mapCharWidth, charPosY + mapCharHeight, 0xAA0000);
		else
			m_engine->DrawScreenRectangle(charPosX, charPosY, charPosX + mapCharWidth, charPosY + mapCharHeight, 0x0000AA);
	}

	int xPos, yPos;
	
	for (int y = 0; y < mapHeight; y++)
	for (int x = 0; x < mapWidth; x++)
		if (GetMapValue(x, y) < 5){
			xPos = 10 + ((double)(x)/ (double)mapWidth) * 250;
			yPos = 10 + ((double)(y) / (double)mapHeight) * 79;
			m_engine->DrawScreenRectangle(xPos, yPos, xPos + mapCharWidth, yPos + mapCharHeight, 0x666666);
		}
	
	charPosX = 10 + ((double)player->GetWorldX() / (double)mapWidth) * 250;
	charPosY = 10 + ((double)player->GetWorldY() / (double)mapHeight) * 79;

	m_engine->DrawScreenRectangle(charPosX, charPosY, charPosX + mapCharWidth, charPosY + mapCharHeight, 0x00AA00);

	m_engine->DrawScreenString(615, 5, sHealth, 0xFFFFFF, NULL);
	m_engine->DrawScreenString(615, 35, sMagic, 0xFFFFFF, NULL);
}

bool Playing::HasCollided(Character* object1, Character* object2){
	//SIMPLE COORDINATE COLLISION
	return(object1->GetWorldX() == object2->GetWorldX() && object1->GetWorldY() == object2->GetWorldY());
}