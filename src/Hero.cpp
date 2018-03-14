#include "header.h"
#include "Hero.h"
#include "LKREngine.h"
#include "Playing.h"

#define SMALL_HEAL 0
#define FULL_HEAL 1
#define FIRE 2
#define FREEZE 3
#define INVISIBILITY 4

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

Hero::Hero(LKREngine* pEngine, Stage* stage, int xSize, int ySize, int worldXPos, int worldYPos)
	: Character(pEngine, stage, xSize, ySize, worldXPos, worldYPos)
{
	spriteRight1.LoadImage("heroRight1.png");
	spriteRight2.LoadImage("heroRight2.png");
	spriteLeft1.LoadImage("heroLeft1.png");
	spriteLeft2.LoadImage("heroLeft2.png");
	spriteUp1.LoadImage("heroUp1.png");
	spriteUp2.LoadImage("heroUp2.png");
	spriteDown1.LoadImage("heroDown1.png");
	spriteDown2.LoadImage("heroDown2.png");
	spriteAttackUp.LoadImage("heroUpAttack.png");
	spriteAttackDown.LoadImage("heroDownAttack.png");
	spriteAttackRight.LoadImage("heroRightAttack.png");
	spriteAttackLeft.LoadImage("heroLeftAttack.png");
	level = 1;
	experience = 0;
	expToLevelUp = 3;
	defencePoints = 5;
	attackPoints = 10;

	sprite = &spriteDown1;
}

Hero::~Hero()
{
}

void Hero::Draw(){
	if (!IsVisible())
		return;

	if (localX < 0 || localX > stage->GetScreenTileWidth() -1 || localY < 0 || localY > stage->GetScreenTileHeight() -1)
		return;

	sprite->RenderImageWithMask(m_pMainEngine->GetForeground(), 0, 0, m_iCurrentScreenX, m_iCurrentScreenY, sprite->GetWidth(), sprite->GetHeight());

	StoreLastScreenPositionForUndraw();

	if (attacking){
		attacking = false;
		SelectSprite();
	}
}


void Hero::DoUpdate(int iCurrentTime){
	if (dead){
		m_pMainEngine->StageSwitch(5);
		return;
	}

	sprite->SetDrawPosition(m_iCurrentScreenX, m_iCurrentScreenY);

	cameraX = stage->GetCameraX();
	cameraY = stage->GetCameraY();

	localX = worldX - cameraX;
	localY = worldY - cameraY;

	if (localX < 0)
		localX = 0;
	if (localX > 15)
		return;
	if (localY < 0)
		localY = 0;
	if (localY > 9)
		return;

	m_iCurrentScreenX = localX * m_iDrawWidth;
	m_iCurrentScreenY = localY * m_iDrawHeight + (m_iDrawHeight * 2);

	RedrawObjects();
}

void Hero::SelectSprite(){ //Determines the sprite to use at a given moment

	switch (direction){
	case UP:
		if (attacking){
			sprite = &spriteAttackUp;
			break;
		}
		if ((frame = (frame + 1) % 2) == 1)
			sprite = &spriteUp1;
		else
			sprite = &spriteUp2;
		break;
	case RIGHT:
		if (attacking){
			sprite = &spriteAttackRight;
			break;
		}
		if ((frame = (frame + 1) % 2) == 1)
			sprite = &spriteRight1;
		else
			sprite = &spriteRight2;
		break;
	case DOWN:
		if (attacking){
			sprite = &spriteAttackDown;
			break;
		}
		if ((frame = (frame + 1) % 2) == 1)
			sprite = &spriteDown1;
		else
			sprite = &spriteDown2;
		break;
	case LEFT:
		if (attacking){
			sprite = &spriteAttackLeft;
			break;
		}
		if ((frame = (frame + 1) % 2) == 1)
			sprite = &spriteLeft1;
		else
			sprite = &spriteLeft2;
		break;
	}


}

void Hero::ProcessInput(int button, int x, int y){
	if (button == SDL_BUTTON_LEFT)
		ProcessInput(SDLK_a);
}

void Hero::ProcessMouseMovement(int x, int y){

	int currY = localY * 50 + 125,
		currX = localX * 50 + 25;
	int vAmount = y - currY,
		hAmount = x - currX;

	int prevDir = direction;
	if (abs(vAmount) > abs(hAmount)){
		if (vAmount >= 0)
			direction = DOWN;
		else
			direction = UP;
	}
	else{
		if (hAmount >= 0)
			direction = RIGHT;
		else
			direction = LEFT;
	}
	if (prevDir != direction){
		SelectSprite();
		RedrawObjects();
	}
}

void Hero::ProcessInput(int keyCode){
	if (keyCode == SDLK_p){
		playStage->TogglePause();
		return;
	}
	
	if (playStage->IsPaused()) return;

	switch (keyCode){
	case SDLK_UP:
		direction = UP;
		SelectSprite();
		if (worldY > 0 && stage->GetMapValue(worldX, worldY - 1) > 4){
			worldY--;
			if (localY <= playStage->GetScreenTileHeight() / 2)
				playStage->SceneTransition(UP, 1);
		}
			break;
	case SDLK_DOWN:
		direction = DOWN;
		SelectSprite();
		if (worldY + 1 < stage->GetMapHeight() - 1 && stage->GetMapValue(worldX, worldY + 1) > 4){
			worldY++;
			if (localY >= playStage->GetScreenTileHeight()/ 2 && keyCode == SDLK_DOWN)	
				playStage->SceneTransition(DOWN, 1);
		}
		break;
	case SDLK_LEFT:
		direction = LEFT;
		SelectSprite();
		if (worldX > 0 && stage->GetMapValue(worldX - 1, worldY) > 4){
			worldX--;
			if (localX <= playStage->GetScreenTileWidth() / 2)
				playStage->SceneTransition(LEFT, 1);
		}
		break;
	case SDLK_RIGHT:
		direction = RIGHT;
		SelectSprite();
		if (worldX + 1 < stage->GetMapWidth() - 1 && stage->GetMapValue(worldX + 1, worldY) > 4){
			worldX++;

			if (localX >= playStage->GetScreenTileWidth() / 2 && keyCode == SDLK_RIGHT)
				playStage->SceneTransition(RIGHT, 1);
		}
		break;
	case SDLK_a: //ATTACK
	{
		int nextX = worldX, nextY = worldY;
		attacking = true;
		SelectSprite();
		switch (direction){
		case UP:
			if (nextY - 1 >= 0)
				nextY -= 1;
			break;
		case RIGHT:
			if (nextX + 1 <= stage->GetMapWidth())
				nextX += 1;
			break;
		case DOWN:
			if (nextY + 1 <= stage->GetMapHeight())
				nextY += 1;
			break;
		case LEFT:
			if (nextX - 1 >= 0)
				nextX -= 1;
			break;
		}

		if (stage->IsCharacterAt(nextX, nextY)){
			int reward = stage->GetCharacterAt(nextX, nextY)->Attack(attackPoints, direction);
			if (reward > 0){
				GiveExperience(reward);
				GiveHealth(reward);
			}
		}
	}
		break;
	case SDLK_s: //SMALL HEAL
		DoMagic(SMALL_HEAL);
		break;
	case SDLK_d: //FULL HEAL
		DoMagic(FULL_HEAL);
		break;
	}
}