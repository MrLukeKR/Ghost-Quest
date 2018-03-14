#pragma once
#include "Character.h"
#include "LKREngine.h"
#include "Stage.h"

class LKREngine;

class Hero :
	public Character
{
public:
	Hero(LKREngine* pEngine, Stage* stage, int xSize, int ySize, int worldXPos, int worldYPos);
	~Hero();

	void Draw();
	void DoUpdate(int iCurrentTime);
	void ProcessInput(int keyCode);
	void ProcessInput(int button, int x, int y);
	void ProcessMouseMovement(int x, int y);

	void SetAttackPoints(int ap){attackPoints = ap;}
	void SetDefencePoints(int dp){ defencePoints = dp; };
	void SetExperience(int exp){ experience = exp; }
	void SetRemainingExperience(int re){ expToLevelUp = re; }
	void SetLevel(int lvl){ level = lvl; }
	void SetHealth(int cHealth){ health = cHealth; }
	void SetMagic(int cMagic){ magic = cMagic; }
	void SetMaxHealth(int mHealth){ maxHealth = mHealth; }
	void SetMaxMagic(int mMagic){ maxMagic = mMagic; }
	void SetWorldX(int cWorldX){worldX = cWorldX;}
	void SetWorldY(int cWorldY){ worldY = cWorldY; }
	void SetLocalX(int cLocalX){ localX = cLocalX; }
	void SetLocalY(int cLocalY){ localY = cLocalY; }

private:
	Sprite spriteRight1;
	Sprite spriteRight2;
	Sprite spriteLeft1;
	Sprite spriteLeft2;
	Sprite spriteUp1;
	Sprite spriteUp2;
	Sprite spriteDown1;
	Sprite spriteDown2;
	Sprite spriteAttackUp;
	Sprite spriteAttackDown;
	Sprite spriteAttackLeft;
	Sprite spriteAttackRight;

	int frame = 0;
	bool attacking = false;
	void SelectSprite();
};

