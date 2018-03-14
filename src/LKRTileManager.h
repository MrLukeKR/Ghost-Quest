#pragma once
#include "tilemanager.h"

class Sprite;
class LKREngine;

class LKRTileManager :
	public TileManager
{
public:
	LKRTileManager();
	~LKRTileManager(void);

	// Draw a tile at a location on the screen determined by the tile
	// position and the base X and Y co-ordinates
	// OVERRIDE THIS TO DRAW YOUR TILE!!!
	// Draw a tile
	virtual void DrawTileAt(
		BaseEngine* pEngine,
		SDL_Surface* pSurface,
		int iMapX, int iMapY,
		int iStartPositionScreenX, int iStartPositionScreenY) const;

	int GetValueAt(int x, int y);

private:
	Sprite* bricks;
	Sprite* grass;
	Sprite* water;
	Sprite* tree;
	Sprite* blockedBrick;
	Sprite* spiderWeb;
};

