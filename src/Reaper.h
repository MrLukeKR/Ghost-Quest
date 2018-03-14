#pragma once
#include "Enemy.h"
class Reaper :
	public Enemy
{
public:
	Reaper(LKREngine* pEngine, Stage* stage, int xSize, int ySize, int worldXPos, int worldYPos, Hero* player);
	~Reaper();
};

