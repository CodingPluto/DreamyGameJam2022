#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <sstream>
//#include <stdio.h>
#include "frame/Game.h"
#include "frame/Sprite.h"

using namespace std;
namespace GameLoopFunctions{
    Game game{};
    vector<Sprite*> mySprites;
    vector<AnimationComponent*> animationSpriteComponents; 
    void Init(){


        game.initalize("My First Game!",1420,580,500,500,false);
        vector<SDL_Texture*> running_textures;
        for (int i = 1; i < 9; ++i){ 
            running_textures.push_back(game.getTexture("src/assets/lucyRunning/" + to_string(i) + ".png"));
        }

        for (int i = 0; i < 100; i++){
            Sprite *sprite = new Sprite(&game);
            sprite->setScale(i/3);
            mySprites.push_back(sprite);
            AnimationComponent *animationComponent = new AnimationComponent(sprite,4,true,1000-i);
            animationComponent->addAnimationTextures(running_textures,"Running");
            animationComponent->setAnimation("Running");
            animationSpriteComponents.push_back(animationComponent);
            mySprites[i]->setY(i*10);
            mySprites[i]->setX(i);
        }
        if (game.isRunning()){
            game.runLoop();
        }

        return;
    }

    void processMainInput(){}

    void updateMainGame(){
        
        for (int i = 0; i < 100; i++){
            mySprites[i]->changeX(5*( 100 - i + 1));
            if (mySprites[i]->getPosition().x > 1920){
                mySprites[i]->setX(0);
                animationSpriteComponents[i]->toggleLooping(false);
            }
        }
    }

    void generateMainOutput(){}
};


int main(int argc, char **argv){
    GameLoopFunctions::Init();


    for (int i = 0; i < GameLoopFunctions::mySprites.size(); i++){
        delete GameLoopFunctions::mySprites[i];
        delete GameLoopFunctions::animationSpriteComponents[i];
    }
    GameLoopFunctions::mySprites.clear();
    GameLoopFunctions::animationSpriteComponents.clear();
    return 0;

}







