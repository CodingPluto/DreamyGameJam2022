#ifndef GAME_
#define GAME_

#include <SDL.h>


class Game{
public:
    Game();
    ~Game();
    void initalize(const char title[], unsigned short xStart, unsigned short yStart, unsigned short width, unsigned short height, bool fullscreen);
    //void render();
    //bool isRunning();
    //void stopGame();

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
};

#endif