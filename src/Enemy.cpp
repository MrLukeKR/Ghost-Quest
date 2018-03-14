#include "header.h"
#include "LKREngine.h"
#include "Enemy.h"
#include <algorithm>

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

using namespace std;

Enemy::Enemy(LKREngine* pEngine, Stage* stage, int xSize, int ySize, int worldXPos, int worldYPos, Hero* player)
	: Character(pEngine, stage, xSize, ySize, worldXPos, worldYPos), player(player)
{
	sprite = &enemy1;

	hOutlineW = 60;
	hOutlineH = 10;
	maxHealth = 100;
}


Enemy::~Enemy()
{
}

void Enemy::Draw(){
	if (!IsVisible())
		return;

	if (worldX < cameraX || worldX >= cameraX + stage->GetScreenTileWidth() || worldY < cameraY || worldY >= cameraY + stage->GetScreenTileHeight())
		return;

	hOutlineX = m_iCurrentScreenX + (m_iDrawWidth / 2) - hOutlineW / 2 - 1;
	hOutlineY = m_iCurrentScreenY - 1.5 * healthH - 1;
	
	float healthPercentage = (float)health / (float)maxHealth;

	healthX = hOutlineX + 1;
	healthY = hOutlineY + 1;
	healthW = (hOutlineW - 2) * healthPercentage;
	healthH = hOutlineH - 2;

	nameX = m_iCurrentScreenX + (m_iDrawWidth / 2) - (nameW / 2);
	nameY = m_iCurrentScreenY + m_iDrawHeight;
	
	drawHealth = CanDraw(hOutlineX, hOutlineY, hOutlineW, hOutlineH);
	drawName = CanDraw(nameX, nameY, nameW, charH);

	if (drawHealth){
		m_pEngine->DrawScreenRectangle(hOutlineX, hOutlineY, hOutlineX + hOutlineW - 1, hOutlineY + hOutlineH - 1, 0xFFFFFF);
		if (healthW > 0)
			m_pEngine->DrawScreenRectangle(healthX , healthY, healthX + healthW -1, healthY + healthH -1, 0XFF0000);
	}

	if (drawName)
		m_pEngine->DrawScreenString(nameX, nameY, name, 0xffffff);

	sprite->RenderImageWithMask(m_pEngine->GetForeground(),0,0,m_iCurrentScreenX, m_iCurrentScreenY,sprite->GetWidth(), sprite->GetHeight());

	StoreLastScreenPositionForUndraw();

	if ((spriteFrame = (spriteFrame + 1) % 4) >=2)
		sprite = &enemy1;
	else
		sprite = &enemy2;
}

void Enemy::DoUpdate(int iCurrentTime){
	if (playStage->IsPaused())
		return;

	if (dead){
		SetVisible(false);
		UndrawHUD();
		RedrawBackground();
		m_pEngine->RemoveDisplayableObject(this);
		return;
	}

	PerformMovement();

	sprite->SetDrawPosition(m_iCurrentScreenX, m_iCurrentScreenY);

	int tHeight = m_pMainEngine->GetTileManager().GetTileHeight(),
		tWidth  = m_pMainEngine->GetTileManager().GetTileWidth();

	int playerX = player->GetLocalX() * tWidth,
		playerY = player->GetLocalY() * tHeight + tHeight * 2;

	if (localX >=0 && localY >=0 && localX < stage->GetScreenTileWidth() && localY < stage->GetScreenTileHeight())
		if (sprite->HasPixelCollidedWithMask(m_iCurrentScreenX, m_iCurrentScreenY, playerX, playerY, player->GetSprite())) {
			int x = player->GetWorldX(), 
				y = player->GetWorldY();

			player->Attack(attackPoints, direction);

			for (int i = 2; i > 0; i--)
				if (x == player->GetWorldX() + i || x == player->GetWorldX() - i || y == player->GetWorldY() + i || y == player->GetWorldY() - i){
					playStage->SceneTransition(direction, i);
					break;
				}
		}

	cameraX = stage->GetCameraX();
	cameraY = stage->GetCameraY();

	localX = worldX - cameraX;
	localY = worldY - cameraY;

	if (worldX < 0)
		worldX = 0;
	if (worldX > stage->GetMapWidth() - 1)
		worldX = stage->GetMapWidth() - 1;
	if (worldY < 0)
		worldY = 0;
	if (worldY > stage->GetMapHeight() - 1)
		worldY = stage->GetMapHeight() - 1;

	m_iCurrentScreenX = localX * m_iDrawWidth;
	m_iCurrentScreenY = localY * m_iDrawHeight + (m_iDrawHeight * 2);

	// Ensure not off the screen
	if (m_iCurrentScreenX < 0)
		m_iCurrentScreenX = 0;
	if (m_iCurrentScreenX >= 800 - m_iDrawWidth)
		m_iCurrentScreenX = 800 - m_iDrawWidth;
	if (m_iCurrentScreenY >= 600 - m_iDrawHeight)
		m_iCurrentScreenY = 600 - m_iDrawHeight;
	if (m_iCurrentScreenY < m_iDrawHeight * 2)
		m_iCurrentScreenY = m_iDrawHeight * 2;

	UndrawHUD();
	RedrawObjects();
}

void Enemy::UndrawHUD(){ //Undraw the text and health bars of enemies
	if (drawName)
		m_pEngine->CopyBackgroundPixels(nameX, nameY, nameW, charH);
	if (drawHealth)
		m_pEngine->CopyBackgroundPixels(hOutlineX, hOutlineY, hOutlineW, hOutlineH);
}

bool Enemy::CanDraw(int x, int y, int w, int h){ //Checks to see if the Enemy is on screen
	return ( w > 0 && x >= 0 && x < m_pEngine->GetScreenWidth() - w && y >= m_iDrawHeight * 2 && y < m_pEngine->GetScreenHeight() - h);
}

void Enemy::DoRandomMovement(){ //Move in a random direction
	int dir = rand() % 4;

	switch (dir){
	case UP:
		direction = UP;
		if (worldY-1 > 0 && stage->GetMapValue(worldX, worldY - 1) >= 5 && !stage->IsEnemyAt(worldX, worldY-1))
			worldY--;
		break;
	case RIGHT:
		direction = RIGHT;
		if (worldX+1 < stage->GetMapWidth()-1 && stage->GetMapValue(worldX + 1, worldY) >= 5 && !stage->IsEnemyAt(worldX + 1, worldY))
			worldX++;
		break;
	case DOWN:
		direction = DOWN;
		if (worldY+1 < stage->GetMapHeight()-1 && stage->GetMapValue(worldX, worldY + 1) >= 5 && !stage->IsEnemyAt(worldX, worldY + 1))
			worldY++;
		break;
	case LEFT:
		direction = LEFT;
		if (worldX-1 > 0 && stage->GetMapValue(worldX - 1, worldY) >= 5 && !stage->IsEnemyAt(worldX - 1, worldY))
			worldX--;
		break;
	}
}

void Enemy::MoveViaPlayer(bool runAway){ //Move towards or away from the player
	float distance[4];
	float sDistance[4];

	for (int i = 0; i < 4; i++)
		if (runAway) distance[i] = FLT_MIN;
		else distance[i] = FLT_MAX;

	float greedyDist;

	if (worldY-1 >= 0 )
		distance[UP] = playStage->GetDistance(worldX, worldY - 1, player);

	if (worldY+1 < stage->GetMapHeight() -1 )
		distance[DOWN] = playStage->GetDistance(worldX, worldY + 1, player);

	if (worldX-1 >= 0 )
		distance[LEFT] = playStage->GetDistance(worldX - 1, worldY, player);

	if (worldX+1 < stage->GetMapWidth() - 1)
		distance[RIGHT] = playStage->GetDistance(worldX + 1, worldY, player);

	memcpy(sDistance, distance, sizeof(float) * 4);
	sort(sDistance, sDistance + 4);

	
	for (int i = 0; i < 4; i++){
		if (runAway)	greedyDist = sDistance[3 - i];
		else			greedyDist = sDistance[i];

		if (greedyDist == INT_MIN || greedyDist == INT_MAX)
			continue;

		if (distance[UP] == greedyDist && stage->GetMapValue(worldX, worldY - 1) >= 5 && !stage->IsEnemyAt(worldX, worldY - 1)) {
			direction = UP;
			worldY--;
			return;
		}

		if (distance[DOWN] == greedyDist && stage->GetMapValue(worldX, worldY + 1) >= 5 && !stage->IsEnemyAt(worldX, worldY + 1)) {
			direction = DOWN;
			worldY++;
			return;
		}

		if (distance[RIGHT] == greedyDist && stage->GetMapValue(worldX + 1, worldY) >= 5 && !stage->IsEnemyAt(worldX + 1, worldY)) {
			direction = RIGHT;
			worldX++;
			return;
		}

		if (distance[LEFT] == greedyDist  && stage->GetMapValue(worldX - 1, worldY) >= 5 && !stage->IsEnemyAt(worldX - 1, worldY)) {
			direction = LEFT;
			worldX--;
			return;
		}
	}

	DoRandomMovement(); //If there's no good moves, do random movement
}