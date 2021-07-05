#include "JC_GameWorld.h"

JC_GameWorld::JC_GameWorld()
{
	mapSrcRect = {0,0,0,0};
	mapDstRect = { 0,0,0,0 };
	sidebarSrcRect = { 0,0,0,0 };
	sidebarDstRect = { 0,0,0,0 };
	timer = { 0,0,0,0 };
	scoreRect = { 0,0,0,0 };
	endScoreRect = { 0,0,0,0 };
	printf("Constructor called \n");
	FileLogger filelogger("log-info.txt");
	Init();
	GameLoop();
}

JC_GameWorld::~JC_GameWorld()
{
	printf("Gameworld closes \n");
	OnClose();
}

void JC_GameWorld::Init()
{
	mapSrcRect = { 0, 0, 480, 272 };
	mapDstRect = { 0, 0, 1920, 1088 };
	sidebarSrcRect = { 0,0,512,960 };
	sidebarDstRect = { 1344,64,512,960 };
	timer = { 1370,865,450, 65 };
	potionSrcRect = { 0,0,32,32 };
	potionDstRect = { 1480,710,32,32 };
	scoreRect = { 100, 70, 175, 70 };
	titleSrcRect = { 0,0, 1920, 1080 };
	titleDstRect = { 0,0, 1920, 1080 };
	endScoreRect = { 780, 630, 400, 150 };
	SDL_Log("All SDL_Rects initialized successfully");
	player.Init(renderer);
	testBullet.Init(renderer);
	bulletList.Init(renderer);
	enemyList.Init(renderer);
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load SDL_Mixer Library");
	}
	else
	{
		SDL_Log("Loaded SDL_Mixer Library");
	}
	if (TTF_Init() < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to initialize TTF");
	}
	soundBackMusic = Mix_LoadMUS("./content/backmusic.mp3");
	Mix_PlayMusic(soundBackMusic, 1);
	soundBulletFire = Mix_LoadWAV("./content/bulletfire.wav");
	soundEnemyDeath = Mix_LoadWAV("./content/enemydeath.wav");
	soundPickup = Mix_LoadWAV("./content/pickup.wav");
	SDL_Log("All music/sounds loaded");

	therok = TTF_OpenFont("content/cheese.ttf", 24);
	scoreSurface = TTF_RenderText_Solid(therok, buffer, white);
	scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
	player.Deserialize("./content/player.json");
}

void JC_GameWorld::GameLoop()
{
	while (done != true)
	{
		startMs = SDL_GetTicks();

		Update();

		endMs = SDL_GetTicks();
		delayMs = frameMs - (endMs - startMs);
		if (delayMs > 40)
			delayMs = 0;
		SDL_Delay(delayMs);
		Render();
		currentFrame++;
		if (activeScreen == 1)
			timerCount++;
	}

}

void JC_GameWorld::Update()
{
	while (SDL_PollEvent(&_event))
	{
		if (_event.type == SDL_QUIT)
			done = true;

		if (_event.type == SDL_KEYDOWN && _event.key.repeat == NULL)
		{
			//Universal events, runs no matter the screen
			switch (_event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				done = true;
				SDL_Log("Escape has been pressed, the game will now close");
				break;
			case SDLK_F10:
				SDL_Log("Toggling fullscreen");
				isFullscreen = SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP;
				SDL_SetWindowFullscreen(window, isFullscreen ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
				break;
			case SDLK_m:
				if (Mix_PlayingMusic() == 0)
				{
					SDL_Log("Unmuting music");
					Mix_PlayMusic(soundBackMusic, -1);
				}
				else
				{
					SDL_Log("Muting music");
					Mix_HaltMusic();
				}
				break;
			case SDLK_n:
				if (Mix_Volume(-1, -1) > 0)
				{
					SDL_Log("Muting sounds");
					Mix_Volume(-1, 0);
				}
				else
				{
					SDL_Log("Unmuting sounds");
					Mix_Volume(-1, soundVolume);
				}
				break;
			case SDLK_h:
				if (soundVolume > 0)
				{
					soundVolume -= 32;
					Mix_Volume(-1, soundVolume);
					SDL_Log("Sound volume is now %i", soundVolume);
				}
				break;
			case SDLK_j:
				if (soundVolume < 128)
				{
					soundVolume += 32;
					Mix_Volume(-1, soundVolume);
					SDL_Log("Sound volume is now %i", soundVolume);
				}
				break;
			case SDLK_k:
				if (musicVolume > 0)
				{
					musicVolume -= 32;
					Mix_VolumeMusic(musicVolume);
					SDL_Log("Music volume is now %i", soundVolume);
				}
				break;
			case SDLK_l:
				if (musicVolume < 128)
				{
					musicVolume += 32;
					Mix_VolumeMusic(musicVolume);
					SDL_Log("Music volume is now %i", soundVolume);
				}
				break;
			}
			//Only runs when on title screen
			if (activeScreen == 0)
			{
				switch (_event.key.keysym.sym)
				{
				case SDLK_SPACE:
					activeScreen = 1;
					SDL_Log("Moving to main screen");
					break;
				}
			}
			//Only runs when in game
			if (activeScreen == 1)
			{
				switch (_event.key.keysym.sym)
				{
				case SDLK_w:
					SDL_Log("W has been pressed");
					player.Input(1);
					break;
				case SDLK_s:
					SDL_Log("S has been pressed");
					player.Input(2);
					break;
				case SDLK_a:
					SDL_Log("A has been pressed");
					player.Input(3);
					break;
				case SDLK_d:
					SDL_Log("D has been pressed");
					player.Input(4);
					break;
				case SDLK_RIGHT:
					if (bulletList.timeBetweenShots <= 0)
					{
						SDL_Log("Right arrow has been pressed");
						bulletList.aListOfBullets[bulletList.currentBullet].ChangePosition(player.dstRect.x + 25, player.dstRect.y + 25);
						bulletList.Input(1);
						Mix_PlayChannel(2, soundBulletFire, 0);
					}
					break;
				case SDLK_LEFT:
					if (bulletList.timeBetweenShots <= 0)
					{
						SDL_Log("Left arrow has been pressed");
						bulletList.aListOfBullets[bulletList.currentBullet].ChangePosition(player.dstRect.x + 25, player.dstRect.y + 25);
						bulletList.Input(2);
						Mix_PlayChannel(-1, soundBulletFire, 0);
					}
					break;
				case SDLK_UP:
					if (bulletList.timeBetweenShots <= 0)
					{
						SDL_Log("Up arrow has been pressed");
						bulletList.aListOfBullets[bulletList.currentBullet].ChangePosition(player.dstRect.x + 25, player.dstRect.y + 25);
						bulletList.Input(3);
						Mix_PlayChannel(-1, soundBulletFire, 0);
					}
					break;
				case SDLK_DOWN:
					if (bulletList.timeBetweenShots <= 0)
					{
						SDL_Log("Down arrow has been pressed");
						bulletList.aListOfBullets[bulletList.currentBullet].ChangePosition(player.dstRect.x + 25, player.dstRect.y + 25);
						bulletList.Input(4);
						Mix_PlayChannel(-1, soundBulletFire, 0);
					}
					break;
				case SDLK_e:
					player.Input(9);
					SDL_Log("E has been pressed");
					break;
				case SDLK_r:
					player.Input(10);
					SDL_Log("R has been pressed");
					break;
				case SDLK_f:
					bulletList.Input(5);
					SDL_Log("F has been pressed");
					break;
				case SDLK_LSHIFT:
					SDL_Log("Left shift has been pressed");
					player.movespeed += 2;
					break;
				}
			}
		}

		if (_event.type == SDL_KEYUP && _event.key.repeat == NULL)
		{
			switch (_event.key.keysym.sym)
			{
			case SDLK_w:
				SDL_Log("W has been unpressed");
				player.Input(5);
				break;
			case SDLK_s:
				SDL_Log("S has been unpressed");
				player.Input(6);
				break;
			case SDLK_a:
				SDL_Log("A has been unpressed");
				player.Input(7);
				break;
			case SDLK_d:
				SDL_Log("D has been unpressed");
				player.Input(8);
				break;
			case SDLK_LSHIFT:
				SDL_Log("Left shift has been unpressed");
				player.movespeed -= 2;
				break;
			}
		}
	}

	if (activeScreen == 1)
	{
		//Update timer
		if (timerCount % 60 == 0)
		{
			timer.w -= 15;
		}
		if (timerCount % 30 == 0)
			SDL_Log("Timer: %i", (60 - (timerCount / 30)));
			//cout << "Timer: " << 60 - (timerCount / 30) << endl;
		if ((60 - (timerCount / 30)) == 0)
		{
			score += player.health * 10;
			activeScreen = 2;
		}


		//Enemies colliding with player
		for (int i = 0; i < enemyList.aListOfEnemies.size(); i++)
			if (Collision(player.dstRect, enemyList.aListOfEnemies[i].dstRect) == true)
			{
				SDL_Log("Player has collided with an enemy");
				player.DealDamage();
				if (player.alive == false)
				{
					SDL_Log("Player has died, moving to screen 2");
					activeScreen = 2;
				}
			}

		//Enemies colliding with bullets
		for (int i = 0; i < bulletList.aListOfBullets.size(); i++)
		{
			for (int j = 0; j < enemyList.aListOfEnemies.size(); j++)
			{
				if (Collision(enemyList.aListOfEnemies[j].dstRect, bulletList.aListOfBullets[i].dstRect) == true)
				{
					SDL_Log("A bullet has collided with an enemy");
					bulletList.aListOfBullets[i].Collided();
					enemyList.aListOfEnemies[j].health -= 25;
					if (enemyList.aListOfEnemies[j].health <= 0)
					{
						Mix_PlayChannel(-1, soundEnemyDeath, 0);
						score += 100;
						SDL_Log("Score: %i", score);
						srand(time(0));
						potion = rand() % 3;
						if (potion == 0)
						{
							SDL_Log("Spawned health potion");
							JC_HealthPotion newHealthPotion;
							newHealthPotion.Init(renderer, enemyList.aListOfEnemies[j].dstRect.x, enemyList.aListOfEnemies[j].dstRect.y);
							aListOfHealth.push_back(newHealthPotion);
						}
						else if (potion == 1)
						{
							SDL_Log("Spawned shotspeed potion");
							JC_ShotspeedPotion newShotPotion;
							newShotPotion.Init(renderer, enemyList.aListOfEnemies[j].dstRect.x, enemyList.aListOfEnemies[j].dstRect.y);
							aListOfShotspeed.push_back(newShotPotion);
						}
						else if (potion == 2)
						{
							SDL_Log("Spawned movespeed potion");
							JC_MovespeedPotion newMovePotion;
							newMovePotion.Init(renderer, enemyList.aListOfEnemies[j].dstRect.x, enemyList.aListOfEnemies[j].dstRect.y);
							aListOfMovespeed.push_back(newMovePotion);
						}
					}
				}
			}
		}

		//Player collision with health potion
		for (int i = 0; i < aListOfHealth.size(); i++)
		{
			if (Collision(player.dstRect, aListOfHealth[i].dstRect) == true)
			{
				SDL_Log("Player has collided with a health potion");
				Mix_PlayChannel(-1, soundPickup, 0);
				score += 50;
				SDL_Log("Score: %i", score);
				player.healthPotions++;
				aListOfHealth.erase(aListOfHealth.begin() + i);
				if (player.healthPotions > 9)
					player.healthPotions = 9;
			}
		}

		//Player collision with shotspeed potion
		for (int i = 0; i < aListOfShotspeed.size(); i++)
		{
			if (Collision(player.dstRect, aListOfShotspeed[i].dstRect) == true)
			{
				SDL_Log("Player has collided with a shotspeed potion");
				Mix_PlayChannel(-1, soundPickup, 0);
				score += 50;
				SDL_Log("Score: %i", score);
				bulletList.shotspeedPotions++;
				aListOfShotspeed.erase(aListOfShotspeed.begin() + i);
				if (bulletList.shotspeedPotions > 9)
					bulletList.shotspeedPotions = 9;
			}
		}

		//Player collision with movespeed potion
		for (int i = 0; i < aListOfMovespeed.size(); i++)
		{
			if (Collision(player.dstRect, aListOfMovespeed[i].dstRect) == true)
			{
				SDL_Log("Player has collided with a movespeed potion");
				Mix_PlayChannel(-1, soundPickup, 0);
				score += 50;
				SDL_Log("Score: %i", score);
				player.movespeedPotions++;
				aListOfMovespeed.erase(aListOfMovespeed.begin() + i);
				if (player.movespeedPotions > 9)
					player.movespeedPotions = 9;
			}
		}

		if (player.alive == true)
			player.Update();
		bulletList.Update();
		enemyList.Update(player.dstRect.x, player.dstRect.y);
	}

	//Update displayed score
	sprintf(buffer, "Score: %i", score);
	scoreSurface = TTF_RenderText_Solid(therok, buffer, white);
	scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
}

void JC_GameWorld::Render()
{
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 0, 20, 255);

	if (activeScreen == 1)
	{
		//Render map
		SDL_RenderCopy(renderer, mapTexture, &mapSrcRect, &mapDstRect);
		//Render sidebar
		SDL_RenderCopy(renderer, sidebarTexture, &sidebarSrcRect, &sidebarDstRect);
		//Render timer
		SDL_SetRenderDrawColor(renderer, 0, 150, 0, 255);
		SDL_RenderFillRect(renderer, &timer);
		//Render inventory amounts
		potionDstRect = { 1480,710,32,32 };
		potionSrcRect.x = player.healthPotions * 32;
		SDL_RenderCopy(renderer, textTexture, &potionSrcRect, &potionDstRect);

		potionDstRect = { 1645,710,32,32 };
		potionSrcRect.x = player.movespeedPotions * 32;
		SDL_RenderCopy(renderer, textTexture, &potionSrcRect, &potionDstRect);

		potionDstRect = { 1810,710,32,32 };
		potionSrcRect.x = bulletList.shotspeedPotions * 32;
		SDL_RenderCopy(renderer, textTexture, &potionSrcRect, &potionDstRect);

		if (player.alive == true)
			player.Render(renderer);
		testBullet.Render(renderer);
		bulletList.Render(renderer);
		enemyList.Render(renderer);
		for (int i = 0; i < aListOfHealth.size(); i++)
			aListOfHealth[i].Render(renderer);
		for (int i = 0; i < aListOfShotspeed.size(); i++)
			aListOfShotspeed[i].Render(renderer);
		for (int i = 0; i < aListOfMovespeed.size(); i++)
			aListOfMovespeed[i].Render(renderer);
		SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
	}
	else if (activeScreen == 0)
		SDL_RenderCopy(renderer, titleTexture, &titleSrcRect, &titleDstRect);
	else if (activeScreen == 2)
	{
		scoreSurface = TTF_RenderText_Solid(therok, buffer, black);
		scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
		SDL_RenderCopy(renderer, endTexture, &titleSrcRect, &titleDstRect);
		SDL_RenderCopy(renderer, scoreTexture, NULL, &endScoreRect);
	}

	SDL_SetRenderDrawColor(renderer, 10, 0, 20, 255);
	SDL_RenderPresent(renderer);
}

bool JC_GameWorld::Collision(SDL_Rect rect1, SDL_Rect rect2)
{
	//Define mins + maxes
	minX1 = rect1.x;
	minX2 = rect2.x;
	maxX1 = rect1.x + rect1.w;
	maxX2 = rect2.x + rect2.w;
	minY1 = rect1.y;
	minY2 = rect2.y;
	maxY1 = rect1.y + rect1.h;
	maxY2 = rect2.y + rect2.h;

	//Compare them to see if there's collision
	if ((maxX1 < minX2) || (minX1 > maxX2) || (maxY1 < minY2) || (minY1 > maxY2))
		return false;
	else
		return true;
}

void JC_GameWorld::OnClose()
{
	
}