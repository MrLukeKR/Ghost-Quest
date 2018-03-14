#pragma once
#include "Enemy.h"
class Spider :
	public Enemy
{
public:
	Spider(LKREngine* pEngine, Stage* stage, int xSize, int ySize, int worldXPos, int worldYPos, Hero* player);
	~Spider();

	void PerformMovement();
private:
	int dir = rand() % 4;
	LKREngine* myEngine;
	LKRTileManager* myTm;
};

