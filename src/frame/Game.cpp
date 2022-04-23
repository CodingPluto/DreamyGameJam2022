#include <SDL.h>
#include <iostream>
#include "Game.h"

using namespace std;


Game::Game(){
    
}
Game::~Game(){
    SDL_Delay(1000);
    SDL_DestroyWindow(window);
    SDL_Quit();

}


void Game::initalize(const char title[], unsigned short xStart, unsigned short yStart, unsigned short width, unsigned short height, bool fullscreen){
    window = SDL_CreateWindow(title,xStart,yStart,width,height,SDL_WINDOW_RESIZABLE);
    cout << "Game Window Created!" << endl;
}