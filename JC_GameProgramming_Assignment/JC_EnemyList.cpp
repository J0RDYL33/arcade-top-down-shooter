#include "JC_EnemyList.h"

JC_EnemyList::JC_EnemyList()
{
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "EnemyList constructed with Param(%p)", this);
}

JC_EnemyList::~JC_EnemyList()
{
	//printf("EnemyList destroyed with Param(%p)", this);
}

void JC_EnemyList::Init(SDL_Renderer* aRenderer)
{
	//Construct 10 enemies
	for (int i = 0; i < 10; i++)
	{
		JC_Enemy newEnemy;
		newEnemy.Init(aRenderer);
		aListOfEnemies.push_back(newEnemy);
	}
}

void JC_EnemyList::Update(int px, int py)
{
	if (timeBetweenSpawns > 0)
		timeBetweenSpawns--;
	else
	{
		timeBetweenSpawns = startTimeBetweenSpawns;
		aListOfEnemies[currentEnemy].SpawnIn();
		currentEnemy++;
	}

	if (currentEnemy % aListOfEnemies.size() == 0)
		currentEnemy = 0;

	for (int i = 0; i < aListOfEnemies.size(); i++)
		aListOfEnemies[i].Update(px, py);
}

void JC_EnemyList::Render(SDL_Renderer* aRenderer)
{
	//Render each of the active bullets
	for (int i = 0; i < aListOfEnemies.size(); i++)
		aListOfEnemies[i].Render(aRenderer);
}