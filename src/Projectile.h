#pragma once
#include "DisplayableObject.h"
#include "Stage.h"
#include "LKREngine.h"
#include "Sprite.h"
#include "Hero.h"

class Projectile :
	public DisplayableObject
{
public:
	Projectile(LKREngine* pEngine, Stage* stage, int worldXPos, int worldYPos, int direction, Character* nPlayer);
	~Projectile();

	void Projectile::Draw();
	void Projectile::DoUpdate(int iCurrentTime);
	bool CanDraw(int x, int y, int w, int h);
	void PerformMovement();
	void RedrawBackground();

protected:
	Sprite sprite;
	LKREngine* lkrEngine;
	Stage* stage;
	Character* character;

	int direction;
	int pixX, pixY;
	int width, height;

	int localX, localY;
	int worldX, worldY;
	int cameraX, cameraY;
};

