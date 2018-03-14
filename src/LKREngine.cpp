#include "header.h"
#include "LKREngine.h"
#include "TileManager.h"
#include "Stage.h"
#include "Menu.h"
#include "Playing.h"
#include "Hero.h"
#include "GameOver.h"

LKREngine::LKREngine() 
	: BaseEngine(50)
{
	stages[6] = new Menu(this);
	//stages[1] = new LoadGame(this);
	//stages[3] = new SaveGame(this);
	stages[5] = new GameOver(this);
	currentStage = stages[6];
}


LKREngine::~LKREngine()
{
	for (int i = 0; i < sizeof(stages); i++)
		delete stages[i];
}

void LKREngine::SetupBackgroundBuffer(){
	int xSize = currentStage->GetScreenTileWidth();
	int ySize = currentStage->GetScreenTileHeight();

	int tmX = currentStage->GetTileManagerStartX();
	int tmY = currentStage->GetTileManagerStartY();
	
	FillBackground(0x000000);

	if (xSize > 0 || ySize > 0){
		std::vector<std::string> data = currentStage->GetCurrentView();

		m_oTiles.SetSize(xSize, ySize);

		for (int x = 0; x < xSize; x++)
			for (int y = 0; y < ySize; y++)
				m_oTiles.SetValue(x, y, data[y][x] - 'a');

		m_oTiles.SetBaseTilesPositionOnScreen(tmX, tmY);

		m_oTiles.DrawAllTiles(this,
			this->GetBackground(),
			0, 0, xSize - 1, ySize - 1);
	}

	currentStage->Draw();
}

void LKREngine::Draw(){
	currentStage->Draw();
}

void LKREngine::DrawStrings(){
	currentStage->DrawStrings();
}

void LKREngine::UnDrawStrings(){
	currentStage->UndrawStrings();
}

void LKREngine::GameAction(){
if (!IsTimeToActWithSleep())
return;

SetTimeToAct(100);

currentStage->DoUpdate(GetTime());
UpdateAllObjects(GetTime());
Draw();
}

void LKREngine::MouseDown(int iButton, int iX, int iY){
	currentStage->ProcessInput(iButton, iX, iY);
}

void LKREngine::KeyDown(int iKeyCode){
	currentStage->ProcessInput(iKeyCode);
}

void LKREngine::StageSwitch(int stageCode){
	DrawableObjectsChanged();
	DestroyOldObjects();
	if (stageCode == 2)
		exit(0);
	if (stageCode == 0)
		stages[0] = new Playing(this,false);
	if (stageCode == 1)
		stages[1] = new Playing(this, true);
	currentStage = stages[stageCode];
	SetupBackgroundBuffer();
	Redraw(true);
}

void LKREngine::MouseMoved(int iX, int iY){
	currentStage->ProcessMouseMovement(iX, iY);
}