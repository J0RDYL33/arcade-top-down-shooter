#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include <iostream>
#include <fstream>
#include "json.hpp"

using namespace std;

class JC_Player
{
public:
	JC_Player();
	~JC_Player();

	using json = nlohmann::json;

	SDL_Surface* mySurface;
	SDL_Texture* myTexture;

	void Init(SDL_Renderer* aRenderer);
	void Init(int px, int py, int pw, int ph, int R, int B, int G);
	void Update();
	void Input(int whichKey);
	void Render(SDL_Renderer* aRenderer);
	void DealDamage();
	void OnClose();
	void Serialize(const std::string& path);
	void Deserialize(const std::string& path);

	SDL_Rect srcRect;
	SDL_Rect dstRect;
	SDL_Rect healthBar;
	Mix_Chunk* soundPlayerDamage = NULL;
	Mix_Chunk* soundItemUse = NULL;
	int spriteSize = 4;
	int spriteFrame = 0;
	int spriteWidth = 17;
	bool up = false, down = false, left = false, right = false;
	int health = 100;
	int damageTime = 0;
	int startDamageTime = 60;
	bool alive = true;
	int healthPotions = 0;
	int movespeedPotions = 0;
	double movespeed = 5;
};

