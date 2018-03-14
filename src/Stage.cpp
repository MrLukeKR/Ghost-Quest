#include "header.h"
#include "Stage.h"
#include "LKREngine.h"
#include "JPGImage.h"
#include <iostream>
#include <fstream>
#include <string>
#include "Character.h"
#include "Enemy.h"

using namespace std;

Stage::Stage(LKREngine* pEngine)
{
	m_engine = pEngine;
	m_engine->SetupBackgroundBuffer();
}


Stage::~Stage()
{
	for (int i = 0; i < screenTileHeight; i++)
		delete &currentView[i];

	currentView.clear();

	for (int i = 0; i < mapHeight; i++)
		delete &mapData[i];

	mapData.clear();
}

void Stage::LoadMapData(char* filename){ //Load in the tile data for the background
	ifstream mapFile;
	string line;
	int itemCount = 0;
	int mapLine = 0;

	mapFile.open(filename,ios::in);
	
	if (!mapFile.is_open()){
		cout << "File failed to open!" << endl;
		return;
	}

	cout << "Reading map data..." << endl;

	while (getline(mapFile, line,',')){
		switch (itemCount++){
		case 0:
			mapName = new char[sizeof(line)];
			sprintf(mapName, line.c_str());
			break;
		case 1:	tileStartX = stoi(line, nullptr, 10);						break;
		case 2:	tileStartY = stoi(line, nullptr, 10);						break;
		case 3:	mapWidth = stoi(line, nullptr, 10);							break;
		case 4:	mapHeight = stoi(line, nullptr, 10);						break;
		case 5:	screenTileWidth = stoi(line, nullptr, 10);					break;
		case 6:	screenTileHeight = stoi(line, nullptr, 10);					break;
		case 7: lPlayerPosX = stoi(line, nullptr, 10);						break;
		case 8: lPlayerPosY = stoi(line, nullptr, 10);						break;
		case 9: wPlayerPosX = stoi(line, nullptr, 10);						break;
		case 10: wPlayerPosY = stoi(line, nullptr, 10);						break;
		case 11: cameraPosX = stoi(line, nullptr, 10);						break;
		case 12: cameraPosY = stoi(line, nullptr, 10);						break;
		default:
			if (line.substr(0, 1) == "\n")
				line.erase(0, 1);	//ERASE THE NEW LINE CHAR
			mapData.push_back(line);
			break;
		}
	}

	mapFile.close();
}

void Stage::UpdateCurrentView(){ //Update the vector that contains the currently visible part of the map
	currentView.clear();
	for (int y = 0; y < screenTileHeight; y++){
		string mapLine(mapData[cameraPosY + y]);
		string currentLine(mapLine.substr(cameraPosX,screenTileWidth));
		currentView.push_back(currentLine);
	}
}

void Stage::PrintVariables(){ //Prints Map data
	cout << "   Map Name: " << mapName << endl;
	cout << "   Local Player Position: (" << lPlayerPosX << "," << lPlayerPosY << ")" << endl;
	cout << "   Tiles Manager Start Point: (" << tileStartX << ", " << tileStartY << ")" << endl;
	cout << "   Map Dimensions: " << mapWidth << " x " << mapHeight << endl;
	cout << "   Tiles per Screen Dimensions: " << screenTileWidth << " x " << screenTileHeight << endl;
	cout << "   Local Player Position: (" << lPlayerPosX << "," << lPlayerPosY << ")" << endl;
	cout << "   World Player Position: (" << wPlayerPosX << "," << wPlayerPosY << ")" << endl;
	cout << "   Camera Position: (" << cameraPosX << "," << cameraPosY << ")" << endl;
}

void Stage::PrintCurrentView(){ //Prints the current view vector
	system("cls");
	for (int y = 0; y < screenTileHeight; y++)
		cout << currentView[y] << endl;
}

void Stage::UpdateMapData(int x, int y, char value){ //Updates the map at a given position
	mapData[y].at(x) = value;
}

float Stage::GetDistance(Character* object1, Character* object2){ //Calculates the SLD between two characters
	return GetDistance(object1->GetWorldX(), object1->GetWorldY(), object2->GetWorldX(), object2->GetWorldY());
}

float Stage::GetDistance(float x1, float y1, Character* object2){ //Calculates the SLD between coordinates and two characters
	return GetDistance(x1, y1, object2->GetWorldX(), object2->GetWorldY());
}

float Stage::GetDistance(float x1, float y1, float x2, float y2){ //Calculates the SLD between coordinates
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

bool Stage::IsEnemyAt(int x, int y){ //Checks if an enemy is present at a given position
	Character* currEnemy;

	for (int i = 1; i < m_engine->GetNumberOfObjects(); i++){
		currEnemy = (Character*)m_engine->GetDisplayableObject(i);
		if (currEnemy->GetWorldX() == x && currEnemy->GetWorldY() == y)
			return true;
	}

	return false;
}

bool Stage::IsCharacterAt(int x, int y){ //Checks if a character is present at a given position
	Character* currCharacter;

	for (int i = 0; i < m_engine->GetNumberOfObjects(); i++){
		currCharacter = (Character*)m_engine->GetDisplayableObject(i);
		if (currCharacter->GetWorldX() == x && currCharacter->GetWorldY() == y)
			return true;
	}

	return false;
}

Character* Stage::GetCharacterAt(int x, int y){ //Returns the character at a given position
	Enemy* currEnemy;

	for (int i = 1; i < m_engine->GetNumberOfObjects(); i++){
		currEnemy = (Enemy*)m_engine->GetDisplayableObject(i);
		if (currEnemy->GetWorldX() == x && currEnemy->GetWorldY() == y)
			return currEnemy;
	}

	return nullptr;
}