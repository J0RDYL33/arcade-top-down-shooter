#include "JC_MovespeedPotion.h"

JC_MovespeedPotion::JC_MovespeedPotion()
{
	mySurface = IMG_Load("");
	myTexture = 0;
}

JC_MovespeedPotion::~JC_MovespeedPotion()
{

}

void JC_MovespeedPotion::Init(SDL_Renderer* aRenderer, int ex, int ey)
{
	int ret = IMG_Init(SDL_INIT_EVERYTHING);
	mySurface = IMG_Load("content/movespeed.png");
	myTexture = SDL_CreateTextureFromSurface(aRenderer, mySurface);
	// srcRect is the source rectangle where we draw FROM
	srcRect = { 0,0,8,15 };
	// distRect is the DESTINATION where we draw TO on the WINDOW
	dstRect = { ex,ey,24,45 };
}

void JC_MovespeedPotion::Render(SDL_Renderer* aRenderer)
{
	SDL_RenderCopy(aRenderer, myTexture, &srcRect, &dstRect);
}
