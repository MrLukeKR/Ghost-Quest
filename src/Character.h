#pragma once
#include "DisplayableObject.h"
#include "LKREngine.h"
#include "JPGImage.h"
#include "MovementPosition.h"
#include "Sprite.h"

class Playing;

class Character :
	public DisplayableObject
{
public:
	Character(LKREngine* pEngine, Stage* stage, int xSize, int ySize, int worldXPos, int worldYPos);
	~Character();
	
	void GiveExperience(unsigned int amount);

	void GiveHealth(unsigned int amount);
	void takeHealth(unsigned int amount);

	int GetLocalX(){ return localX; }
	int GetLocalY(){ return localY; }
	int GetWorldX(){ return worldX; }
	int GetWorldY(){ return worldY; }

	void SetCameraX(int x){ cameraX = x; }
	void SetCameraY(int y){ cameraY = y; }
	void RedrawBackground();
	void UndrawStrings();

	int GetLevel(){ return level; }
	int GetDefencePoints(){ return defencePoints; }
	int GetAttackPoints(){ return attackPoints; }
	int GetExperiencePoints(){ return experience; }
	int GetRemainingExperience(){ return expToLevelUp; }
	float GetHealthPercentage(){ return (double)health / (double)maxHealth; }
	float GetMagicPercentage(){ return (double)magic / (double)maxMagic; }
	int GetHealth(){ return health; }
	int GetMaxHealth(){ return maxHealth; }
	int GetMagic(){ return magic; }
	int GetMaxMagic(){ return maxMagic; }
	bool IsOnScreen();

	int Attack(int ap, int dir);
	void DoMagic(int spell);

	Sprite* GetSprite(){ return sprite; }

protected:
	LKREngine* m_pMainEngine;
	MovementPosition m_oMover;
	Stage* stage;
	Playing* playStage;
	Sprite* sprite;

	void LevelUp();
	int localX, localY;
	int worldX, worldY;
	int cameraX, cameraY;

	int hOutlineX, hOutlineY, hOutlineW, hOutlineH;
	int healthX, healthY, healthW, healthH;

	char name[7];
	int nameX, nameY,nameW;
	int charW = 18, charH = 25;
	bool drawName = false;

	unsigned int maxHealth = 100, maxMagic = 30;
	unsigned int health = 100, magic = 30;
	bool drawHealth = false, drawMagic = false;

	unsigned int level = 1;
	unsigned int experience = 0;
	int expToLevelUp = 3;

	unsigned int attackPoints = 10;
	unsigned int defencePoints = 5;

	virtual bool CanDraw(int x, int y, int w, int h){ return false; }

	bool dead = false;
	int direction;
};

