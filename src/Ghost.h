#pragma once
#include "Enemy.h"
class Ghost :
	public Enemy
{
public:
	Ghost(LKREngine* pEngine, Stage* stage, int xSize, int ySize, int worldXPos, int worldYPos, Hero* player);
	~Ghost();
};

