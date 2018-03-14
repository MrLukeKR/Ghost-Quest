#pragma once
#include "Character.h"
#include "Playing.h"
#include "Hero.h"

class Enemy :
	public Character
{
public:
	Enemy(LKREngine* pEngine, Stage* stage, int xSize, int ySize, int worldXPos, int worldYPos, Hero* player);
	~Enemy();

	void Draw();
	void DoUpdate(int iCurrentTime);
	void UndrawHUD();

protected:
	bool CanDraw(int x, int y, int w, int h);
	void DoRandomMovement();
	void MoveViaPlayer(bool runAway);
	virtual void PerformMovement();
	int frame = 0;
	int frameWait = 50;
	int perceptionDistance = 5;
	int speed = 10;

	Sprite enemy1;
	Sprite enemy2;

	Hero* player;
	int spriteFrame = 0;
};