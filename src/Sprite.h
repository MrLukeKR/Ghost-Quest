#pragma once
#include "JPGImage.h"
#include "LKREngine.h"

class Sprite :
	public ImageData
{
public:
	Sprite();
	~Sprite();

	bool HasPixelCollidedWithMask(int drawX1, int drawY1, int drawX2, int drawY2, Sprite* object);
	bool HasPixelCollidedWithMask(Sprite* object2);
	
	void SetDrawPosition(int x, int y){ drawX = x; drawY = y; }
	int GetDrawX(){ return drawX; }
	int GetDrawY(){ return drawY; }
private:
	bool PixelExistsAt(int x, int y);
	int drawX, drawY;
};

