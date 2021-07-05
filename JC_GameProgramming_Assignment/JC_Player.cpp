#include "JC_Player.h"

JC_Player::JC_Player()
{
    srcRect = { 1,1,1,1 };
    dstRect = { 1,1,1,1 };
    healthBar = { 1,1,1,1 };
    mySurface = IMG_Load("");
    myTexture = 0;
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Player constructed with Param(%p)", this);
}

JC_Player::~JC_Player()
{
    OnClose();
    printf("Player destroyed with Param(%p)\n", this);
}

void JC_Player::Init(SDL_Renderer* aRenderer)
{
    int ret = IMG_Init(SDL_INIT_EVERYTHING);
    mySurface = IMG_Load("content/fire2.png");
    myTexture = SDL_CreateTextureFromSurface(aRenderer, mySurface);
    if (ret == 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load SDL_Image Library");
    }

    if (mySurface == NULL)
    {
        SDL_Log("Null pointer returned while loading content/fire2.png");
    }
    else
    {
        SDL_Log("Loaded fire2.png");
    }

    if (myTexture == NULL)
    {
        SDL_Log("myTexture was null, that's bad!");
    }
    else
    {
        SDL_Log("Loaded texture from SDL Surface for fire2.png");
    }
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load SDL_Mixer Library");
    }
    else
    {
        SDL_Log("Loaded SDL_Mixer Library");
    }


    // srcRect is the source rectangle where we draw FROM
    srcRect = { 8,16,16,16 };
    // distRect is the DESTINATION where we draw TO on the WINDOW
    dstRect = { 672,544,50,50 };
    //Initialize health bar
    healthBar = { 1370,365,450, 65};

    soundPlayerDamage = Mix_LoadWAV("./content/damagetaken.wav");
    soundItemUse = Mix_LoadWAV("./content/itemuse.wav");
}

void JC_Player::Init(int px, int py, int pw, int ph, int R, int B, int G)
{

}

void JC_Player::Input(int whichKey)
{
    switch (whichKey)
    {
    case 1:
        up = true;
        break;
    case 2:
        down = true;
        break;
    case 3:
        left = true;
        break;
    case 4:
        right = true;
        break;
    case 5:
        up = false;
        break;
    case 6:
        down = false;
        break;
    case 7:
        left = false;
        break;
    case 8:
        right = false;
        break;
    case 9:
        if (healthPotions > 0)
        {
            Mix_PlayChannel(-1, soundItemUse, 0);
            health += 5;
            healthPotions--;
            SDL_Log("Health is now %i. Potions remaining: %i", health, healthPotions);
        }
        break;
    case 10:
        if (movespeedPotions > 0)
        {
            Mix_PlayChannel(-1, soundItemUse, 0);
            movespeed += 0.2;
            movespeedPotions--;
            SDL_Log("Movespeed is now %i. Potions remaining: %i", movespeed, movespeedPotions);
        }
        break;
    }
}

void JC_Player::Update()
{
    //Movable space = {64, 128, 1216, 832}
    if (up == false)
        dstRect.y = dstRect.y;
    if (down == false)
        dstRect.y = dstRect.y;
    if (left == false)
        dstRect.y = dstRect.y;
    if (right == false)
        dstRect.y = dstRect.y;

    if (up == true && dstRect.y > 128)
        dstRect.y -= movespeed;
    else if (down == true && dstRect.y < 910)
        dstRect.y += movespeed;

    if (left == true && dstRect.x > 64)
    {
        dstRect.x -= movespeed;
        srcRect.y = 0;
    }
    else if (right == true && dstRect.x < 1226)
    {
        dstRect.x += movespeed;
        srcRect.y = 16;
    }

    if (up == true || down == true || left == true || right == true)
        spriteFrame++;
    
    if (damageTime > 0)
        damageTime--;

    //Stop health going over 100
    if (health > 100)
        health = 100;
    //Update healthbar
    healthBar.w = 4.5 * health;
}

void JC_Player::DealDamage()
{
    if (damageTime <= 0)
    {
        health -= 20;
        damageTime = startDamageTime;
        SDL_Log("New player health: %i", health);
        Mix_PlayChannel(-1, soundPlayerDamage, 0);
    }

    if (health <= 0 && alive == true)
    {
        alive = false;
    }
}

void JC_Player::Render(SDL_Renderer* aRenderer)
{
    if (spriteFrame % 8 == 0)
        spriteFrame = 0;
    if (spriteFrame == 0 || spriteFrame == 1)
        srcRect.x = 8 + (32 * 0);
    else if (spriteFrame == 2 || spriteFrame == 3)
        srcRect.x = 8 + (32 * 1);
    else if (spriteFrame == 4 || spriteFrame == 5)
        srcRect.x = 8 + (32 * 2);
    else if (spriteFrame == 6 || spriteFrame == 7)
        srcRect.x = 8 + (32 * 3);

    SDL_RenderCopy(aRenderer, myTexture, &srcRect, &dstRect);
    SDL_SetRenderDrawColor(aRenderer, 150, 20, 20, 255);
    SDL_RenderFillRect(aRenderer, &healthBar);
}

void JC_Player::OnClose()
{
    IMG_Quit();
    Mix_CloseAudio();
}

void JC_Player::Serialize(const std::string& path)
{
    //The output stream (to a file)
    std::ofstream outStream;
    //Open the file for writing
    outStream.open(path, std::ios::out);
    //Create an empty json object that we're going to save
    json outObject;
    //Set up fields to save
    outObject["movespeed"] = this->movespeed;
    outObject["position"]["x"] = this->dstRect.x;
    outObject["position"]["y"] = this->dstRect.y;
    outObject["invulnerabilityTime"] = this->startDamageTime;
    //Write to the file -- use dump to get the JSON text
    outStream << outObject.dump(4);
    //And close the file! Very important!
    outStream.close();
}

void JC_Player::Deserialize(const std::string& path)
{
    //The input stream (to a file)
    std::ifstream inStream;
    //Open the file for reading
    inStream.open(path, std::ios::in);
    //Build an empty JSON object that we're going to save into
    json j;
    //Read all JSON data into this object..
    //json.hpp will do the parsing!
    inStream >> j;
    //Now that it's parsed, set the instance variables of the
    //player to whatever is in the parsed JSON object
    this->movespeed = j["movespeed"].get<double>();
    this->startDamageTime = j["invulnerabilityTime"].get<int>();
    this->dstRect.x = j["position"]["x"].get<float>();
    this->dstRect.y = j["position"]["y"].get<float>();
    //And close the file
    inStream.close();
}