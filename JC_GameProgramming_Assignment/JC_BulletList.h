#pragma once
#include "SDL.h"
#include "JC_Bullet.h"
#include "SDL_mixer.h"
#include <vector>
using namespace std;
class JC_BulletList
{
public:
	JC_BulletList();
	void Init(SDL_Renderer* aRenderer);
	void Update();
	void Input(int whichKey);
	void Render(SDL_Renderer* aRenderer);
	~JC_BulletList();
	void OnClose();

	std::vector<JC_Bullet> aListOfBullets;
	Mix_Chunk* soundItemUse = NULL;
	int currentBullet;
	double timeBetweenShots = 0;
	double startTimeBetweenShots = 15;
	int shotspeedPotions = 0;
};

