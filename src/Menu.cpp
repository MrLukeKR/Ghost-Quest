#include "header.h"
#include "Menu.h"
#include "LKREngine.h"

Menu::Menu(LKREngine* pEngine) : m_engine(pEngine)
{
	titleImage.LoadImage("ghostQuestTitle.png");
}


Menu::~Menu()
{

}

void Menu::ProcessMouseMovement(int x, int y){ //Change menu selection based on mouse location
	if (x >= 550 && y >= 300 && x < 800 && y < 320){
		menuSelection = 0;
		m_engine->Redraw(true);
}
	if (x >= 550 && y >= 335 && x < 800 && y < 355){
		menuSelection = 1;
		m_engine->Redraw(true);
	}
	if (x >= 550 && y >= 370 && x < 800 && y < 390){
		menuSelection = 2;
		m_engine->Redraw(true);
	}

};

void Menu::ProcessInput(int button, int x, int y){
	if (button != 1)
		return;

	if (x >= 550 && y >= 300 && x < 800 && y < 320){
		menuSelection = 0;
		m_engine->Redraw(true);
	}
	if (x >= 550 && y >= 335 && x < 800 && y < 355){
		menuSelection = 1;
		m_engine->Redraw(true);
	}
	if (x >= 550 && y >= 370 && x < 800 && y < 390){
		menuSelection = 2;
		m_engine->Redraw(true);
	}

	m_engine->StageSwitch(menuSelection);
}

void Menu::DrawStrings(){
	m_engine->CopyBackgroundPixels(0, 0, m_engine->GetScreenWidth(), 50);
	if (menuSelection == 0)
		m_engine->DrawScreenString(550, 300, "New Game", 0xFFFFFF, NULL);
	else
		m_engine->DrawScreenString(550, 300, "New Game", 0x000000, NULL);

	if (menuSelection == 1)
		m_engine->DrawScreenString(550, 335, "Load Game", 0xFFFFFF, NULL);
	else
		m_engine->DrawScreenString(550, 335, "Load Game", 0x000000, NULL);

	if (menuSelection == 2)
	m_engine->DrawScreenString(550, 370, "Quit", 0xFFFFFF, NULL);
	else
		m_engine->DrawScreenString(550, 370, "Quit", 0x000000, NULL);

	m_engine->DrawScreenString(20, 510, "Arrow Keys = Move, Mouse = Rotate", 0x000000, NULL);
	m_engine->DrawScreenString(20, 530, "A/Left Click = Sword", 0x770000, NULL);
	m_engine->DrawScreenString(20, 550, "S = Small Heal", 0x000077, NULL);
	m_engine->DrawScreenString(20, 570, "D = Full Heal", 0x000077, NULL);

	m_engine->DrawScreenString(530, 550, "ESC = Save", 0x000000, NULL);
	m_engine->DrawScreenString(530, 570, "Q = Save and Quit", 0x000000, NULL);
}

void Menu::Draw(){
	int cWidth = titleImage.GetWidth();
	int cHeight = titleImage.GetHeight();

	titleImage.RenderImage(m_engine->GetBackground(), 0, 0,0,0, cWidth, cHeight);
}

void Menu::ProcessInput(int keyCode){
	switch (keyCode){
	case SDLK_DOWN:
		if (menuSelection < 2)
			menuSelection++;
		break;
	case SDLK_UP:
		if (menuSelection > 0)
			menuSelection--;
		break;
	case SDLK_RETURN:
		m_engine->StageSwitch(menuSelection);
		break;
	case SDLK_ESCAPE:
		exit(0);
		break;
	}

	selectionY = selectionBaseY + (menuSelection * 25);
	m_engine->Redraw(true);
}