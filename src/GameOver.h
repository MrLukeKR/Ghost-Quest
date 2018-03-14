#pragma once
#include "Stage.h"
#include "JPGImage.h"

class LKREngine;

class GameOver :
	public Stage
{
public:
	GameOver(LKREngine* pEngine);
	~GameOver();

	void Draw();
	void DrawStrings();
	void ProcessInput(int keyCode);

protected:
	LKREngine* m_engine;

private:
	ImageData oSkull, skull;

};

