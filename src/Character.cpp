#include "header.h"
#include "Character.h"
#include <algorithm>
#include "Projectile.h"

#define SMALL_HEAL 0
#define FULL_HEAL 1

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

using namespace std;

Character::Character(LKREngine* pEngine, Stage* nStage, int xSize, int ySize, int worldXPos, int worldYPos)
	: DisplayableObject(pEngine), worldX(worldXPos), worldY(worldYPos), m_pMainEngine(pEngine)
{
	stage = nStage;
	playStage = (Playing*)nStage;

	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;

	m_iDrawWidth = xSize;
	m_iDrawHeight = ySize;

	cameraX = stage->GetCameraX();
	cameraY = stage->GetCameraY();

	localX = worldX - cameraX;
	localY = worldY - cameraY;

	m_iPreviousScreenX = m_iCurrentScreenX = xSize * localX ;
	m_iPreviousScreenY = m_iCurrentScreenY = ySize * localY + (ySize * 2);

	direction = DOWN;

	SetVisible(true);
}


Character::~Character()
{
}

void Character::GiveHealth(unsigned int amount){ //Add to the character's health value
	if (health + amount > maxHealth)
		amount = maxHealth;
	else
		health += amount;
}

void Character::GiveExperience(unsigned int amount){ //Add to the character's experience value
	experience += amount;
	expToLevelUp -= amount;

	if (expToLevelUp <= 0){
		int remaining = abs(expToLevelUp);
		LevelUp();
		expToLevelUp -= remaining;
	}
}

void Character::LevelUp(){ //Increment the character's level and recalculate their statistics
	level++;
	expToLevelUp = level * level;
	defencePoints += level;
	attackPoints += level;
	maxHealth = 90 + level * 10;
	maxMagic = 25 + level * 5;
	health = maxHealth;
	magic = maxMagic;
}

int Character::Attack(int ap, int dir){ //Apply damage to this character
	unsigned int attVar = ap / 2;
	unsigned int attMin = ap - attVar;
	unsigned int attMax = ap + attVar;

	unsigned int defVar = defencePoints / 2;
	unsigned int defMin = defencePoints - defVar;
	unsigned int defMax = defencePoints + defVar;

	int damage = (rand() % (attMax - attMin + 1) + attMin) - (rand() %(defMax - defMin + 1) + defMin);

	if (damage < 0)
		damage = 0;

	if ((int)health - damage < 0)
		health = 0;
	else
		health -= damage;

	switch (dir){
	case UP:
		for (int i = 2; i >= 0; i --)
			if (worldY - i > 0 && stage->GetMapValue(worldX, worldY - i) >= 5){
				worldY -= i;
				break;
			}
		break;
	case RIGHT:
		for (int i = 2; i >= 0; i--)
			if (worldX + i < stage->GetMapWidth()-1 && stage->GetMapValue(worldX + i, worldY) >= 5){
				worldX += i;
				break;
			}
		break;
	case DOWN:
		for (int i = 2; i >= 0; i--)
			if (worldY + i < stage->GetMapHeight()-1 && stage->GetMapValue(worldX, worldY + i) >= 5){
				worldY += i;
				break;
			}
		break;
	case LEFT:
		for (int i = 2; i >= 0; i--)
			if (worldX - i > 0 && stage->GetMapValue(worldX - i, worldY) >= 5){
				worldX -= i;
				break;
			}
		break;
	}
	if (health == 0){
		dead = true;
		return level;
	}
	else
		return 0;
}

void Character::DoMagic(int spell){ //Perform a spell (Healing) [In future could be fire, freeze, invisible etc.]
	int mpCost = 0;
	
	switch (spell){
	case FULL_HEAL:
		mpCost = maxHealth - health;
		if (mpCost > magic)	return;
		health += mpCost;
		break;
	case SMALL_HEAL:
		mpCost = min((int)maxHealth - (int)health, 10);
		if (mpCost > magic) return;
		health += mpCost;
		break;
	}

	magic -= mpCost;
}

void Character::RedrawBackground()
{
	if (localX < -1 || localX > 16 || localY < -1 || localY > 16)
			return;

	if (m_iPreviousScreenX < 0 || m_iPreviousScreenY < 0 || m_iPreviousScreenX > m_pEngine->GetScreenWidth() - m_iDrawWidth || m_iPreviousScreenY > m_pEngine->GetScreenHeight() - m_iDrawHeight)
		return;

	m_pEngine->CopyBackgroundPixels(m_iPreviousScreenX + m_iStartDrawPosX,
		m_iPreviousScreenY + m_iStartDrawPosY, m_iDrawWidth, m_iDrawHeight);
}

bool Character::IsOnScreen(){ //Checks is a character is on the screen at the point of calling
	return (localX >= 0 && localY >= 0 && localX < stage->GetScreenTileWidth() && localY < stage->GetScreenTileWidth());
}