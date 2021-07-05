#include "JC_BulletList.h"

JC_BulletList::JC_BulletList()
{
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "BulletList constructed with Param(%p)", this);
}

JC_BulletList::~JC_BulletList()
{
	printf("BulletList destroyed with Param(%p)", this);
}

void JC_BulletList::Init(SDL_Renderer* aRenderer)
{
	//Construct 10 bullets
	for (int i = 0; i < 10; i++)
	{
		JC_Bullet newBullet;
		newBullet.Init(aRenderer);
		aListOfBullets.push_back(newBullet);
	}
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load SDL_Mixer Library");
	}
	else
	{
		SDL_Log("Loaded SDL_Mixer Library");
	}
	soundItemUse = Mix_LoadWAV("./content/itemuse.wav");
}

void JC_BulletList::Input(int whichKey)
{
	//Called when the player fires, calls the next bullet object in the vectors update to change it's location
	if (timeBetweenShots <= 0)
	{
		switch (whichKey)
		{
		case 1:
			timeBetweenShots = startTimeBetweenShots;
			aListOfBullets[currentBullet].right = true;
			aListOfBullets[currentBullet].active = true;
			currentBullet++;
			break;
		case 2:
			timeBetweenShots = startTimeBetweenShots;
			aListOfBullets[currentBullet].left = true;
			aListOfBullets[currentBullet].active = true;
			currentBullet++;
			break;
		case 3:
			timeBetweenShots = startTimeBetweenShots;
			aListOfBullets[currentBullet].up = true;
			aListOfBullets[currentBullet].active = true;
			currentBullet++;
			break;
		case 4:
			timeBetweenShots = startTimeBetweenShots;
			aListOfBullets[currentBullet].down = true;
			aListOfBullets[currentBullet].active = true;
			currentBullet++;
			break;
		case 5:
			if (shotspeedPotions > 0)
			{
				Mix_PlayChannel(-1, soundItemUse, 0);
				startTimeBetweenShots -= 0.5;
				shotspeedPotions--;
				cout << "Shot speed is now: " << startTimeBetweenShots << ". Potions remaining: " << shotspeedPotions << endl;
				SDL_Log("Shot speed is now: %i. Potions remaining: %i", startTimeBetweenShots, shotspeedPotions);
			}
			break;
		}
	}
}

void JC_BulletList::Update()
{
	//Counts down between bullet fires
	for (int i = 0; i < aListOfBullets.size(); i++)
		aListOfBullets[i].Update();

	if (currentBullet % aListOfBullets.size() == 0)
		currentBullet = 0;

	if (timeBetweenShots > 0)
		timeBetweenShots -= 1;
}

void JC_BulletList::Render(SDL_Renderer* aRenderer)
{
	//Render each of the active bullets
	for (int i = 0; i < aListOfBullets.size(); i++)
		aListOfBullets[i].Render(aRenderer);
}

void JC_BulletList::OnClose()
{
	//Destruct each bullet
}