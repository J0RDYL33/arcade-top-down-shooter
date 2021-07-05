#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include <iostream>
using namespace std;

class JC_Enemy
{
public:
	JC_Enemy();
	~JC_Enemy();
	SDL_Surface* mySurface;
	SDL_Texture* myTexture;

	void Init(SDL_Renderer* aRenderer);
	void Init(int px, int py, int pw, int ph, int R, int B, int G);
	void Update(int playerX, int playerY);
	void Input(int whichKey);
	void Render(SDL_Renderer* aRenderer);
	void SpawnIn();
	void SpawnOut();

	SDL_Rect srcRect;
	SDL_Rect dstRect;
	int spriteSize = 4;
	int spriteFrame = 0;
	int spriteWidth = 17;
	bool active = false;
	int spawnPoint = 0;
	int health = 0;
	int startHealth = 100;
};

