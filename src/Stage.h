#pragma once

#include "LKRTileManager.h"

class Character;
class LKREngine;

class Stage
{
public:
	Stage(){};
	Stage(LKREngine* pEngine);
	~Stage();

	virtual void DrawStrings(){};
	virtual void UndrawStrings(){};

	std::vector<std::string> GetCurrentView(){ return currentView; }
	void UpdateCurrentView();
	void UpdateMapData(int x, int y, char value);

	void LoadMapData(char* filename);
	void LoadGame(char* filename){};

	virtual void InitialiseObjects(){};
	virtual void ProcessInput(int keyCode){};
	virtual void ProcessInput(int button, int x, int y){};
	virtual void ProcessMouseMovement(int x, int y){};
	virtual void Draw(){};
	
	int GetScreenTileHeight(){ return screenTileHeight; }
	int GetScreenTileWidth(){ return screenTileWidth; }

	int GetTileManagerStartX(){ return tileStartX; }
	int GetTileManagerStartY(){ return tileStartY; }

	int GetMapHeight(){ return mapHeight; }
	int GetMapWidth(){ return mapWidth; }

	int GetCameraX(){ return cameraPosX; }
	int GetCameraY(){ return cameraPosY; }

	int GetMapValue(int x, int y){return mapData[y][x] - 'a';}

	float GetDistance(Character* object1, Character* object2);
	float GetDistance(float x1, float y1, Character* object2);
	float GetDistance(float x1, float y1, float x2, float y2);

	bool IsCharacterAt(int x, int y);
	bool IsEnemyAt(int x, int y);
	Character* GetCharacterAt(int x, int y);

	
	virtual void DoUpdate(int iCurrentTime){};

protected:
	BaseEngine* m_engine;
	std::vector<std::string> currentView;
	std::vector<std::string> mapData;
	char* mapName;
	int mapHeight = 0, mapWidth = 0;
	int screenTileHeight= 0, screenTileWidth = 0;
	int cameraPosX=0, cameraPosY =0;
	int wPlayerPosX=0, wPlayerPosY=0, lPlayerPosX=0, lPlayerPosY=0;
	int tileStartX = 0, tileStartY = 0;

	void PrintVariables();
	void PrintCurrentView();
};

