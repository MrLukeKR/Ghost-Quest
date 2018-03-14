#include "header.h"
#include "Spider.h"

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

Spider::Spider(LKREngine* pEngine, Stage* stage, int xSize, int ySize, int worldXPos, int worldYPos, Hero* player)
	:Enemy(pEngine, stage, xSize, ySize, worldXPos, worldYPos, player)
{
	enemy1.LoadImage("spider1.png");
	enemy2.LoadImage("spider2.png");

	perceptionDistance = 0;

	strcpy(name, "SPIDY");

	nameW = strlen(name) * charW;

	speed = 15;
	frameWait = 25 - speed;

	m_iCurrentScreenX = 0;
	m_iCurrentScreenY = 0;

	myEngine = (LKREngine*)m_pEngine;
	myTm = &myEngine->GetTileManager();
}


Spider::~Spider()
{
}

void Spider::PerformMovement(){ //Move in straight lines until an obstacle is hit (Spiders can walk on webs and start another line randomly whilst on webs)
	if (playStage->IsPaused())
		return;

	if (frame++ % frameWait > 0)
		return;

	stage->UpdateMapData(worldX, worldY, 'a');

	if (worldX >= cameraX && worldX < cameraX + stage->GetScreenTileWidth() && worldY >= cameraY && worldY < cameraY + stage->GetScreenTileHeight()){
		stage->UpdateCurrentView();

		//UPDATE STAGE
		for (int x = 0; x < stage->GetScreenTileWidth(); x++)
			for (int y = 0; y < stage->GetScreenTileHeight(); y++)
				myTm->UpdateTile(m_pEngine, x, y, stage->GetCurrentView()[y][x] - 'a');

		m_pEngine->Redraw(true);
	}
	
	int mapVal = 0;

	switch (dir){
	case UP:
		direction = UP;
		mapVal = stage->GetMapValue(worldX, worldY - 1);
		if (worldY - 1 > 0 && (mapVal >= 5 || mapVal == 0) && !stage->IsEnemyAt(worldX, worldY - 1))
			worldY--;
		else
			dir = rand() % 4;
		break;
	case RIGHT:
		direction = RIGHT;
		mapVal = stage->GetMapValue(worldX + 1, worldY);
		if (worldX + 1 < stage->GetMapWidth() - 1 && (mapVal >= 5 || mapVal == 0) && !stage->IsEnemyAt(worldX + 1, worldY))
			worldX++;
		else
			dir = rand() % 4;
		break;
	case DOWN:
		direction = DOWN;
		mapVal = stage->GetMapValue(worldX, worldY + 1);
		if (worldY + 1 < stage->GetMapHeight() - 1 && (mapVal >= 5 || mapVal == 0) && !stage->IsEnemyAt(worldX, worldY + 1))
			worldY++;
		else
			dir = rand() % 4;
		break;
	case LEFT:
		direction = LEFT;
		mapVal = stage->GetMapValue(worldX - 1, worldY);
		if (worldX - 1 > 0 && (mapVal >= 5 || mapVal == 0) && !stage->IsEnemyAt(worldX - 1, worldY))
			worldX--;
		else
			dir = rand() % 4;
		break;
	}

	if (mapVal==0)
		dir = rand() % 4;
}