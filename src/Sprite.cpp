#include "header.h"
#include "Sprite.h"
#include <algorithm>

using namespace std;

Sprite::Sprite()
	:ImageData()
{
}


Sprite::~Sprite()
{
}

bool Sprite::PixelExistsAt(int x, int y){
	int iMask = m_aiPixels[GetHeight() *GetWidth()- 1];
	
	return m_aiPixels[x + y * GetWidth() - 1] != iMask;
}

bool Sprite::HasPixelCollidedWithMask(int drawX1, int drawY1, int drawX2, int drawY2, Sprite* object){ //Pixel based collision
	int amount = 0;
	if (drawX1 > drawX2)
		amount = GetWidth();
	int x2 = min(drawX1 + GetWidth()-1, drawX2 + object->GetWidth()-1),
		x1 = max(drawX1 , drawX2 ),
		y2 = min(drawY1 + GetHeight() -1, drawY2 + object->GetHeight()-1),
		y1 = max(drawY1 , drawY2 );

	if (x2 - x1 < 0 || y2 - y1 < 0)
		return false;

	if (x1 == x2 && y1 == y2)
		if (drawX1 != drawX2 && drawY1 != drawY2)
			return false;

	int xLoc, yLoc;

	for (int iY = y1; iY < y2; iY++)
		for (int iX = x1; iX < x2; iX++)
			if (PixelExistsAt(x2-iX, y2-iY) && object->PixelExistsAt(x1- iX, y1-iY))
				return true;

	return false;
}

bool Sprite::HasPixelCollidedWithMask(Sprite* object){
	return HasPixelCollidedWithMask(drawX, drawY, object->GetDrawX(), object->GetDrawY(), object);
}