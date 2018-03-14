#pragma once
#include "Stage.h"
#include "JPGImage.h"

class LKREngine;

class Menu :
	public Stage
{
public:
	Menu(LKREngine* pEngine);
	~Menu();

	void DrawStrings();
	char** GetBackground(){ return background; }
	void ProcessInput(int keyCode);
	void ProcessInput(int button, int x, int y);
	void ProcessMouseMovement(int x, int y);
	void Draw();


protected:
	LKREngine* m_engine;
	char** background;
	int menuSelection = 0;
	int selectionBaseY = 340;
	int selectionY = selectionBaseY;

private:
	ImageData titleImage;
};

