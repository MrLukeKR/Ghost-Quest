#pragma once
#include "BaseEngine.h"
#include "LKRTileManager.h"
#include "Stage.h"
#include "Menu.h"

class LKREngine :
	public BaseEngine
{
public:
	LKREngine();
	~LKREngine();

	void SetupBackgroundBuffer();
	void DrawStrings();
	void UnDrawStrings();
	void GameAction();
	void MouseDown(int iButton, int iX, int iY);
	void KeyDown(int iKeyCode);
	void StageSwitch(int stageCode);
	void MouseMoved(int iX, int iY);

	LKRTileManager& GetTileManager() { return m_oTiles; }

private:
	void Draw();
	LKRTileManager m_oTiles;
	Stage* currentStage;

	Stage* stages[7];
};