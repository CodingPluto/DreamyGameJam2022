#include <SDL.h>
#include <iostream>
#include "Game.h"
#include "SDL.h"
#include "SDL_image.h"
#include "Sprite.h"

using namespace std;


void Game::processInput(){
    SDL_Event event;
    while (SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_QUIT:
                exitGame();
                break;
        }
    }
    keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_ESCAPE] || (keys[SDL_SCANCODE_LCTRL] && keys[SDL_SCANCODE_C])){
        exitGame();
    }
}



void Game::updateGame(){
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticksThisFrame + 16));
    deltaTime = (SDL_GetTicks() - ticksThisFrame) / 1000.0;
    ticksThisFrame = SDL_GetTicks();
    if (deltaTime > (float) 0.05) deltaTime = (float) 0.05;
    updatingSprites = true;
    for (auto sprite : sprites){
        sprite->updateSprite(deltaTime);
        sprite->update();
    }
    for (auto pendingSprite : pendingSprites){
        sprites.emplace_back(pendingSprite);
    }
    pendingSprites.clear();
    std::vector<Sprite*> deadSprites;
    for (auto sprite : sprites){
        if (sprite->getState() == Sprite::Dead){
            deadSprites.emplace_back(sprite);
        }
    }
    for (auto deadSprite : deadSprites){
        delete deadSprite;
    }
    deadSprites.clear();
    //SDL_SetRenderDrawColor(renderer, 255, 255, 0, 0);
    // now update game objects
}
void Game::generateOutput(){
    for (auto imageComponent : imageComponents){
        imageComponent->draw(renderer);
    }
    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);
    outputDelta();
}


void Game::outputDelta(){
    cout << "Delta: " << deltaTime << endl;
}

bool Game::isRunning(){
    return gameRunning;
}
Game::Game(){
    cout << "Game Started." << endl;
}
Game::~Game(){
    cout << "Destructing game instance" << endl;
    if (gameRunning){
        exitGame();
    }
}
void Game::exitGame(){
    cout << "Exiting Game!" << endl;
    gameRunning = false;
    clearData();
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_QuitSubSystem(0);
    SDL_Quit();    
}

void Game::clearData(){}; // Instances are automatically removed from vectors at end of program. Just for new pointers really.



SDL_Texture* Game::getTexture(const std::string& textureName){

    SDL_Texture* texture = nullptr;

    auto it = textureMap.find(textureName);
    if (it != textureMap.end()) texture = it->second;
    else{
        SDL_Surface* surf = IMG_Load(textureName.c_str());
        if (!surf){
            cout << "Couldn't find file! Make sure path is included" << endl;
            return nullptr;
        }
        texture = SDL_CreateTextureFromSurface(renderer,surf);
        SDL_FreeSurface(surf);
        if (!texture){
            cout << "Couldn't convert from a surface to a texture." << endl;
            return nullptr;
        }
        textureMap[textureName] = texture;
    }
    return texture;
}


float Game::getDeltaTime(){
    return deltaTime;
};
void Game::initalize(const char title[], unsigned short xStart, unsigned short yStart, unsigned short width, unsigned short height, bool fullscreen){
    if (fullscreen) window = SDL_CreateWindow(title,xStart,yStart,width,height,SDL_WINDOW_FULLSCREEN);
    else window = SDL_CreateWindow(title,xStart,yStart,width,height,SDL_WINDOW_RESIZABLE);
    IMG_Init(IMG_INIT_PNG);
    cout << "Game Window Created!" << endl;
    renderer = SDL_CreateRenderer(window, -1, 0);
    gameRunning = true;
}
void Game::AddImage(ImageComponent *imageComponent){
    auto it = imageComponents.begin();
    for (; it < imageComponents.end(); ++it){
        if (imageComponent->getDrawOrder() < (*it)->getDrawOrder()){
            break;
        }
    }
    imageComponents.insert(it, imageComponent);
}

void Game::RemoveImage(ImageComponent *ImageComponent){
    cout << "Removing Image" << endl;
    auto it = imageComponents.begin();
    while(it != imageComponents.end() && *it != ImageComponent){++it;}
    if (it != imageComponents.end()){
        imageComponents.erase(it);
    }
    cout << "Removed Image" << endl;
}


void Game::AddSprite(Sprite *sprite){
    if (updatingSprites){
        pendingSprites.emplace_back(sprite);
    }
    else{
        sprites.emplace_back(sprite);
    }
}

void Game::RemoveSprite(Sprite *sprite){
    cout << "Removing Sprite" << endl;

    auto pendingIt = pendingSprites.begin();
    while(pendingIt != pendingSprites.end() && *pendingIt != sprite){++pendingIt;}
    if (pendingIt != pendingSprites.end()){
        iter_swap(pendingIt, pendingSprites.end() - 1);
		pendingSprites.pop_back();
        cout << "Removed Sprite" << endl;
        return;
    }
    auto activeIt = sprites.begin();
    while(activeIt != sprites.end() && *activeIt != sprite){++activeIt;}
    if (activeIt != sprites.end()){
        iter_swap(activeIt,sprites.end() - 1);
        sprites.pop_back();
        cout << "Removed Sprite" << endl;
        return;
    }
    cout << "Removed Sprite" << endl;
}

void Game::outputSprites(){
    for (auto it = sprites.begin(); it < sprites.end(); ++it){
        cout << "Sprite " << (it - sprites.begin()) << ": " << *it << endl;
    }
}
