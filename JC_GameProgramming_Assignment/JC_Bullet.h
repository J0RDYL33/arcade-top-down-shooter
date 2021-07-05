#pragma once
#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_image.h"
#include <iostream>
class JC_Bullet
{
public:
	JC_Bullet();
	~JC_Bullet();
	SDL_Surface* mySurface;
	SDL_Texture* myTexture;

	void Init(SDL_Renderer* aRenderer);
	void Init(SDL_Renderer* aRenderer, int px, int py);
	void Update();
	void Input(int whichKey);
	void Render(SDL_Renderer* aRenderer);
	void ChangePosition(int px, int py);
	void Collided();
	void OnClose();

	SDL_Rect srcRect;
	SDL_Rect dstRect;
	int spriteSize = 4;
	bool active = false;
	bool left, right, up, down;
};

