#pragma once
#include "SDL.h"
#include "SDL_image.h"
class JC_ShotspeedPotion
{
public:
	JC_ShotspeedPotion();
	~JC_ShotspeedPotion();

	SDL_Surface* mySurface;
	SDL_Texture* myTexture;

	void Init(SDL_Renderer* aRenderer, int ex, int ey);
	void Render(SDL_Renderer* aRenderer);

	SDL_Rect srcRect;
	SDL_Rect dstRect;
};


