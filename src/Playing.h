#pragma once
#include "Stage.h"
#include "Hero.h"

class LKREngine;

class Playing :
	public Stage
{
public:
	Playing(LKREngine* pEngine, bool loadGame);
	~Playing();

	void InitialiseObjects();
	void Draw();
	void DrawStrings();
	void SceneTransition(int direction, unsigned int amount);
	void ProcessInput(int keyCode);
	void ProcessMouseMovement(int x, int y);
	void ProcessInput(int button, int x, int y);
	bool HasCollided(Character* object1, Character* object2);
	void DoUpdate(int iCurrentTime);
	void TogglePause() { paused = !paused; }
	bool IsPaused(){ return paused; }

	Hero* GetPlayer(){ return player; }

private:
	LKREngine* m_engine;
	Hero* player;
	unsigned int frame = 0;
	int currentScene;

	void SaveGame();
	void LoadGame();
	
	int hXStart = 560;
	int mXStart = 560;

	int hYStart = 7;
	int mYStart = 37;
	int hWidth = 225;
	int mWidth = 225;

	int mapCharWidth;
	int mapCharHeight;

	int xMax, xMin, yMax, yMin;

	void GenerateEnemies(int amount);
	void GenerateSpider();
	void GenerateReaper();

	bool paused = false;
	int pauseFlash = 0;
	int saveFlash = 0;
	int loadFlash = 0;
};


