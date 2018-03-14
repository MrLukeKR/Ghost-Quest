#include "header.h"
#include "LKRTileManager.h"
#include "Sprite.h"

LKRTileManager::LKRTileManager()
	: TileManager(50, 50)
{
	bricks = new Sprite();
	grass = new Sprite();
	water = new Sprite();
	tree = new Sprite();
	blockedBrick = new Sprite();
	spiderWeb = new Sprite();

	bricks->LoadImage("brick.png");
	grass->LoadImage("grass.png");
	water->LoadImage("water.png");
	tree->LoadImage("tree.png");
	blockedBrick->LoadImage("blockedbrick.png");
	spiderWeb->LoadImage("spiderweb.png");
}


LKRTileManager::~LKRTileManager()
{
	delete bricks;
	delete grass;
	delete water;
	delete tree;
	delete blockedBrick;
	delete spiderWeb;
}

void LKRTileManager::DrawTileAt(
	BaseEngine* pEngine,
	SDL_Surface* pSurface,
	int iMapX, int iMapY,
	int iStartPositionScreenX, int iStartPositionScreenY) const
{
	Sprite* currSprite = NULL;

	int value = GetValue(iMapX, iMapY);

	//0-4 are blocking tiles

	switch (value)
	{
	case 0: //SPIDER WEB (a)
		currSprite = spiderWeb;
		break;
	case 1: //BLOCKING BRICK (b)
		currSprite = blockedBrick;
		break;
	case 3: //TREE (d)
		currSprite = tree;
		break;
	case 4: //WATER (e)
		currSprite = water;
		break;
	case 5: //GRASS (f)
		currSprite = grass;
		break;
	case 8: //BRICK (i)
		currSprite = bricks;
		break;
	}

	if (currSprite != NULL)
		currSprite->RenderImage(pEngine->GetBackground(), 0, 0, iStartPositionScreenX, iStartPositionScreenY, m_iTileWidth, m_iTileHeight);
}

int LKRTileManager::GetValueAt(int x, int y){
	int tileX, tileY;
	tileX = x / GetTileWidth();
	tileY = y / GetTileHeight();

	return GetValue(tileX, tileY);
}