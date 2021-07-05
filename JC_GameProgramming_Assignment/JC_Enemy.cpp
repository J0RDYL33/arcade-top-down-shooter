#include "JC_Enemy.h"

JC_Enemy::JC_Enemy()
{
    srcRect = { 1,1,1,1 };
    dstRect = { 1,1,1,1 };
    mySurface = IMG_Load("");
    myTexture = 0;
    SDL_Log("Enemy constructed with Param(%p)", this);
}

JC_Enemy::~JC_Enemy()
{
    //SDL_Log("Enemy destroyed with Param(%p)\n", this);
}

void JC_Enemy::Init(SDL_Renderer* aRenderer)
{
    int ret = IMG_Init(SDL_INIT_EVERYTHING);
    mySurface = IMG_Load("content/lightning.png");
    myTexture = SDL_CreateTextureFromSurface(aRenderer, mySurface);
    /*if (ret == 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load SDL_Image Library");
    }

    if (mySurface == NULL)
    {
        SDL_Log("Null pointer returned while loading content/lightning.png");
    }
    else
    {
        SDL_Log("Loaded lightning.png");
    }

    if (myTexture == NULL)
    {
        SDL_Log("myTexture was null, that's bad!");
    }
    else
    {
        SDL_Log("Loaded texture from SDL Surface for lightning.png");
    }*/


    // srcRect is the source rectangle where we draw FROM
    srcRect = { 0,0,16,16 };
    // distRect is the DESTINATION where we draw TO on the WINDOW
    dstRect = { -100,400,50,50 };
}

void JC_Enemy::Init(int px, int py, int pw, int ph, int R, int B, int G)
{

}

void JC_Enemy::Input(int whichKey)
{

}

void JC_Enemy::Update(int playerX, int playerY)
{
    if (active == true)
    {
        spriteFrame++;

        if (playerX > dstRect.x)
            dstRect.x += 2;
        else
            dstRect.x -= 2;
        if (playerY > dstRect.y)
            dstRect.y += 2;
        else
            dstRect.y -= 2;
    }

    if (health <= 0)
    {
        SpawnOut();
    }
}

void JC_Enemy::SpawnIn()
{
    SDL_Log("Enemy spawned in");
    active = true;
    srand(time(0));
    spawnPoint = rand() % 4;
    switch (spawnPoint)
    {
    case (0):
        //672, 128
        dstRect.x = 672;
        dstRect.y = 128;
        break;
    case (1):
        //64, 480
        dstRect.x = 64;
        dstRect.y = 480;
        break;
    case (2):
        //1230, 480
        dstRect.x = 1230;
        dstRect.y = 480;
        break;
    case (3):
        //672, 910
        dstRect.x = 672;
        dstRect.y = 910;
        break;
    }
}

void JC_Enemy::SpawnOut()
{
    SDL_Log("Enemy spawned out");
    active = false;
    dstRect.x = -100;
    dstRect.y = 400;
    health = startHealth;
}


void JC_Enemy::Render(SDL_Renderer* aRenderer)
{
    if (spriteFrame % 8 == 0)
        spriteFrame = 0;
    if (spriteFrame == 0 || spriteFrame == 1)
        srcRect.x = 0;
    else if (spriteFrame == 2 || spriteFrame == 3)
        srcRect.x = 16;
    else if (spriteFrame == 4 || spriteFrame == 5)
        srcRect.x = 32;
    else if (spriteFrame == 6 || spriteFrame == 7)
        srcRect.x = 48;

    SDL_RenderCopy(aRenderer, myTexture, &srcRect, &dstRect);
}