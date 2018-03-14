#include "header.h"
#include "GameOver.h"
#include "LKREngine.h"

GameOver::GameOver(LKREngine* pEngine) : m_engine(pEngine)
{
	oSkull.LoadImage("skull.png");
	skull.ShrinkFrom(&oSkull, 2);
}


GameOver::~GameOver()
{
}


void GameOver::Draw(){
	skull.RenderImageWithMask(m_engine->GetBackground(), 0, 0, m_engine->GetScreenWidth() / 2 - skull.GetWidth() / 2, m_engine->GetScreenHeight() / 3 - skull.GetHeight() / 2, skull.GetWidth(), skull.GetHeight());
}

void GameOver::DrawStrings(){
	m_engine->DrawScreenString(325, 350,"Game Over",0xFFFFFF);

	m_engine->DrawScreenString(220, 450, "Press Enter to Continue", 0xFFFFFF);
}

void GameOver::ProcessInput(int keyCode){
	if (keyCode == SDLK_RETURN)
		m_engine->StageSwitch(6);
}