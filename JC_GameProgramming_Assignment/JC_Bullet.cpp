#include "JC_Bullet.h"

JC_Bullet::JC_Bullet()
{
	SDL_Surface* mySurface = IMG_Load("");
	SDL_Texture* myTexture = 0;
	srcRect = { 1,1,1,1 };
	dstRect = { 1,1,1,1 };
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Bullet constructed with Param(%p)", this);
}

JC_Bullet::~JC_Bullet()
{
	printf("Bullet destroyed with Param(%p)\n", this);
}

void JC_Bullet::Init(SDL_Renderer* aRenderer)
{
    int ret = IMG_Init(SDL_INIT_EVERYTHING);
    mySurface = IMG_Load("content/bullet.png");
    myTexture = SDL_CreateTextureFromSurface(aRenderer, mySurface);
    /*if (ret == 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load SDL_Image Library");
    }

    if (mySurface == NULL)
    {
        SDL_Log("Null pointer returned while loading content/bullet.png");
    }
    else
    {
        SDL_Log("Loaded bullet.png");
    }

    if (myTexture == NULL)
    {
        SDL_Log("myTexture was null, that's bad!");
    }
    else
    {
        SDL_Log("Loaded texture from SDL Surface for bullet.png");
    }*/


    // srcRect is the source rectangle where we draw FROM
    srcRect = { 0,0,432,432 };
    // distRect is the DESTINATION where we draw TO on the WINDOW
    dstRect = { -100,100,20,20 };
}

void JC_Bullet::Update()
{
    if (active == true)
    {
        if (right == true)
        {
            dstRect.x += 10;
        }
        else if (left == true)
        {
            dstRect.x -= 10;
        }
        else if (up == true)
        {
            dstRect.y -= 10;
        }
        else if (down == true)
        {
            dstRect.y += 10;
        }
    }

    if (active == true && (dstRect.x <= 64 || dstRect.x >= 1260 || dstRect.y <= 128 || dstRect.y >= 876))
        Collided();
}

void JC_Bullet::Input(int whichKey)
{

}

void JC_Bullet::ChangePosition(int px, int py)
{
    dstRect.x = px;
    dstRect.y = py;
}

void JC_Bullet::Collided()
{
    active = false;
    up = false; down = false; left = false; right = false;
    ChangePosition(-100, 100);
}

void JC_Bullet::Render(SDL_Renderer* aRenderer)
{
    SDL_RenderCopy(aRenderer, myTexture, &srcRect, &dstRect);
}

void JC_Bullet::OnClose()
{

}