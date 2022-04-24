#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
//#include <stdio.h>
#include "frame/Game.h"
#include "frame/Sprite.h"

using namespace std;

int main(int argc, char **argv){

    Game game{};
    game.initalize("My First Game!",1420,580,500,500,false);
    SDL_Texture* texture = game.getTexture("src/assets/lucy.png");

    Sprite myRectangle{&game};
    ImageComponent rectangleComp{&myRectangle};
    rectangleComp.setTexture(texture);

    myRectangle.setPosition(Vector2{300,200});
    myRectangle.setRotation(1.4);
    
    if (game.isRunning()){
        game.runLoop();
    }

    return 0;

}












