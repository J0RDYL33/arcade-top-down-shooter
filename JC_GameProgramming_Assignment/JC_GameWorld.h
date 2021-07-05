#pragma once
#define _CRT_NONSTDC_NO_DEPRECATE
#include "SDL.h"
#include <vector>
#include "SDL_image.h"
#include "SDL_mixer.h"
#include <iostream>
#include <stdio.h>
#include <ostream>
#include <math.h>
#include <fstream>
#include <string>
#include "FileLogger.h"
#include "JC_Player.h"
#include "JC_Enemy.h"
#include "JC_Bullet.h"
#include "JC_BulletList.h"
#include "JC_EnemyList.h"
#include "JC_HealthPotion.h"
#include "JC_ShotspeedPotion.h"
#include "JC_MovespeedPotion.h"
#include "SDL_ttf.h"
using namespace std;

class JC_GameWorld
{
public:
	JC_GameWorld();
	~JC_GameWorld();


	SDL_Window* window = SDL_CreateWindow("Jordan Cave - CGP2015M - CAV19695697 - Escape the Reality", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1020, SDL_WINDOW_RESIZABLE + SDL_WINDOW_MAXIMIZED);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Event _event;

	bool done = false;
	Uint32 startMs, endMs, delayMs = 0;
	int framerate = 30;
	Uint32 frameMs = 1000 / framerate;
	int currentFrame = 0;
	bool gKeysB[256];
	int gKeysI[256];
	bool isFullscreen = SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP;
	int maxX1, maxX2, minX1, minX2, maxY1, maxY2, minY1, minY2;
	int score = 0;
	int potion = 0;
	int timerCount;
	int musicVolume = 128;
	int soundVolume = 128;
	char buffer[256];
	int activeScreen = 0;

	const int DELTA_TIME = 50;

	//Game Objects
	//SDL_Rect testRect;
	JC_Player player;
	JC_Bullet testBullet;
	JC_BulletList bulletList;
	JC_EnemyList enemyList;
	std::vector<JC_HealthPotion> aListOfHealth;
	std::vector<JC_ShotspeedPotion> aListOfShotspeed;
	std::vector<JC_MovespeedPotion> aListOfMovespeed;
	//items: Health potion, speed up, shot speed up
	SDL_Surface* mapSurface = IMG_Load("content/map.png");
	SDL_Texture* mapTexture = SDL_CreateTextureFromSurface(renderer, mapSurface);
	SDL_Surface* sidebarSurface = IMG_Load("content/sidebar.png");
	SDL_Texture* sidebarTexture = SDL_CreateTextureFromSurface(renderer, sidebarSurface);
	SDL_Rect timer;
	SDL_Rect mapSrcRect;
	SDL_Rect mapDstRect;
	SDL_Rect sidebarSrcRect;
	SDL_Rect sidebarDstRect;
	SDL_Surface* textSurface = IMG_Load("content/numbers.png");
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_Rect potionSrcRect = { 0,0,0,0 };
	SDL_Rect potionDstRect = { 0,0,0,0 };
	//Music + sounds
	Mix_Music* soundBackMusic = NULL;
	Mix_Chunk* soundBulletFire = NULL;
	Mix_Chunk* soundEnemyDeath = NULL;
	Mix_Chunk* soundPickup = NULL;
	//Score on screen
	TTF_Font* therok = TTF_OpenFont("content/cheese.ttf", 24);
	SDL_Color white = { 255, 255, 255 };
	SDL_Color black = { 0, 0, 0 };
	SDL_Surface* scoreSurface = TTF_RenderText_Solid(therok, buffer, white);
	SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
	SDL_Rect scoreRect;
	SDL_Rect endScoreRect;
	//Title screen
	SDL_Surface* titleSurface = IMG_Load("content/titlescreen.png");
	SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
	SDL_Rect titleSrcRect = { 0,0,0,0 };
	SDL_Rect titleDstRect = { 0,0,0,0 };
	//End screen
	SDL_Surface* endSurface = IMG_Load("content/endscreen.png");
	SDL_Texture* endTexture = SDL_CreateTextureFromSurface(renderer, endSurface);

	//Functions
	void Init();
	void GameLoop();
	bool Collision(SDL_Rect rect1, SDL_Rect rect2);
	void Update();
	void Render();
	void OnClose();
};

