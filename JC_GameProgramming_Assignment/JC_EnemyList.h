#pragma once
#include "SDL.h"
#include "JC_Enemy.h"
#include <vector>

class JC_EnemyList
{
public:
	JC_EnemyList();
	void Init(SDL_Renderer* aRenderer);
	void Update(int px, int py);
	void Input(int whichKey);
	void Render(SDL_Renderer* aRenderer);
	~JC_EnemyList();
	void OnClose();

	std::vector<JC_Enemy> aListOfEnemies;
	int currentEnemy = 0;
	double timeBetweenSpawns = 0;
	double startTimeBetweenSpawns = 75;
};