#include "header.h"
#include "Ghost.h"
#include <algorithm>

using namespace std;

Ghost::Ghost(LKREngine* pEngine, Stage* stage, int xSize, int ySize, int worldXPos, int worldYPos, Hero* player)
	:Enemy(pEngine, stage, xSize, ySize, worldXPos, worldYPos, player)
{
	enemy1.LoadImage("ghost1.png");
	enemy2.LoadImage("ghost2.png");

	int pLvl = player->GetLevel();
	int minLvl = max(pLvl - 3, 1);
	int maxLvl = pLvl * 3;
	int newLevel = rand() % (maxLvl - minLvl + 1) + minLvl;

	for (int i = 0; i < newLevel - 1; i++)
		LevelUp();

	char sName[7] = "lvl ";
	char nLevel[3];

	itoa(level, nLevel, 10);
	strcat(sName, nLevel);

	strcpy(name, sName);

	nameW = strlen(name) * charW;

	speed = min((int)log(level * health), 24);
	frameWait = 25 - speed;

	m_iCurrentScreenX = 0;
	m_iCurrentScreenY = 0;

	perceptionDistance = 5 + level;
}


Ghost::~Ghost()
{
}

void Enemy::PerformMovement(){ //Move towards player, away from player or randomly
	if (playStage->IsPaused())
		return;

	if (frame++ % frameWait > 0)
		return;

	int dist = stage->GetDistance(this, player);
	float healthPercentage = (float)health / (float)maxHealth;

	if (dist <= perceptionDistance)
		MoveViaPlayer(healthPercentage <= 0.2 || player->GetLevel() > level);
	else
		DoRandomMovement();
}