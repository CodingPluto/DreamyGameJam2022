#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <memory>
#include "frame/Game.h"
#include "frame/Sprite.h"

using namespace std;




class Square : public Sprite{
    private:


    public:
        ImageComponent *image = nullptr;
        PhysicsComponent *physics = nullptr;

        void update(){
            changePosition(100,100);
        }


        Square(Game *game,bool collider = false):Sprite(game){
            image = new ImageComponent(this);
            image->setTexture(game->getTexture("src/assets/lucy.png"));
            physics = new PhysicsComponent(this);
        }
        


        ~Square(){
            delete image;
            delete physics;
        }

};




int main(int argc, char **argv){
    cout << "Program Running!" << endl;
    Game game;
    game.initalize("This is my game",0,0,700,500,false);
    Square *square = new Square(&game);


    game.processInput();
    while(game.isRunning()){
        game.processInput();
        game.updateGame();
        game.generateOutput();
    }
    delete square;
    return 0;
}