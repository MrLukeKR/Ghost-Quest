#include "header.h"
#include "Reaper.h"
#include <algorithm>

using namespace std;


Reaper::Reaper(LKREngine* pEngine, Stage* stage, int xSize, int ySize, int worldXPos, int worldYPos, Hero* player)
	:Enemy(pEngine, stage, xSize, ySize, worldXPos, worldYPos, player)
{
	enemy1.LoadImage("reaper1.png");
	enemy2.LoadImage("reaper2.png");

	for (int i = 0; i < 99; i++)
		LevelUp();

	perceptionDistance = INT_MAX;

	strcpy(name, "GRIM");

	nameW = strlen(name) * charW;

	speed = 20;
	frameWait = 25 - speed;

	m_iCurrentScreenX = 0;
	m_iCurrentScreenY = 0;

}


Reaper::~Reaper()
{
}
