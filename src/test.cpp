#include <iostream>
//#include <SDL.h>
//#include <stdio.h>
#include "frame/Game.h"

using namespace std;

int main(int argc, char *argv[]){

    Game game{};
    game.initalize("My First Game!",0,0,300,500,false);
    return 0;
}


















/*
int main(int argc, char* argv[]) {

    SDL_Window *window;                    // Declare a pointer

    SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2

    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        "An SDL2 window",                  // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        1920,                               // width, in pixels
        1080,                               // height, in pixels
        SDL_WINDOW_OPENGL                  // flags - see below
    );

    // Check that the window was successfully created
    if (window == NULL) {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window,-1,0);

    for (int i = 0; i < 255; ++i){
        SDL_SetRenderDrawColor(renderer, i, i, i, i);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(0.5);
    }
    SDL_Delay(100);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
*/
/*
int main(int argc, char *argv[]){
    cout << "!!!Program Running!!!" << endl;
}*/