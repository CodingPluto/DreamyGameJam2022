#include <iostream>

#include "frame/Game.h"
#include "frame/Sprite.h"
#include "Cloud.h"
#include "Player.h"
#include "frame/audio.h"
#include <vector>
using namespace std;

struct Timer
{
    Uint64 previous_ticks{};
    float elapsed_seconds{};

    void tick()
    {   
        const Uint64 current_ticks{ SDL_GetPerformanceCounter() };
        const Uint64 delta{ current_ticks - previous_ticks };
        previous_ticks = current_ticks;
        static const Uint64 TICKS_PER_SECOND{ SDL_GetPerformanceFrequency() };
        elapsed_seconds = delta / static_cast<float>(TICKS_PER_SECOND);
    }
};

class MousePointer : public Sprite{
    public:
        ImageComponent* image;
        CollisionComponent* collider;
        MousePointer(Game *game): Sprite(game){
            image = new ImageComponent(this);
            collider = new CollisionComponent(this);
            image->setTexture(game->getTexture("src/assets/square.png"));
        }
        ~MousePointer(){
            delete image;
            delete collider;
        }
    private:
        void update(float deltaTime){
            int posX = position.x;
            int posY = position.y;
            SDL_GetGlobalMouseState(&posX,&posY);
            position.x = posX - 500;
            position.y = posY - 350;
            cout << "position X: " << position.x << endl;
            if (collider->colliding){
                cout << "Touching Cloud!" << endl;
            }
        }
};




int main(int argc, char *argv[]){
    cout << "Program Running!" << endl;
    LucyGame game;
    game.initalize("Lucy Dreams",500,500,1920,1080,true);

    //MousePointer mousePointer(&game);
    Cloud::gameReference = &game;
    game.processInput();

    vector<SDL_Texture*> backgrounds = {
        game.getTexture("src/assets/background/sprite_0.png"),
        game.getTexture("src/assets/background/sprite_1.png"),
        game.getTexture("src/assets/background/sprite_2.png"),
        game.getTexture("src/assets/background/sprite_3.png"),
        game.getTexture("src/assets/background/sprite_4.png"),
        game.getTexture("src/assets/background/sprite_5.png"),
        game.getTexture("src/assets/background/sprite_6.png"),
        game.getTexture("src/assets/background/sprite_7.png"),
        game.getTexture("src/assets/background/sprite_8.png"),
        game.getTexture("src/assets/background/sprite_9.png"),
    };
    Cloud::setupCloudImages();


    Sprite background(&game);
    ImageComponent backgroundImage(&background,1);
    backgroundImage.setTexture(game.getTexture("src/assets/background.png"));
    background.setScale(6);



    const int UPDATE_FREQUENCY{ 60 };
    const double CYCLE_TIME{ (double)1.0 / UPDATE_FREQUENCY };
    // System timing
    static Timer system_timer;
    double accumulated_seconds{ (double)0.0 };

    initAudio();
    unsigned long long musicSinceStart = 8880;

    game.processInput();
    Sprite ending(&game);
    ImageComponent endingImage(&ending);

    Sprite titleScreen(&game);
    titleScreen.setScale(10);
    ImageComponent titleCard(&titleScreen);
    titleCard.setTexture(game.getTexture("src/assets/titleScreen.png"));
    titleScreen.setPosition(game.getDisplayWidth()/2 - titleCard.getTextureWidth()/2,game.getDisplayHeight()/2 - titleCard.getTextureHeight()/2);
    Player* player;
    float titleScreenHoverOffset = 0;

    vector<SDL_Texture*> backClouds = {
        game.getTexture("src/assets/backClouds/sprite_0.png"),
        game.getTexture("src/assets/backClouds/sprite_1.png"),
        game.getTexture("src/assets/backClouds/sprite_2.png"),
        game.getTexture("src/assets/backClouds/sprite_3.png"),
        game.getTexture("src/assets/backClouds/sprite_4.png"),
    };

    vector<Sprite*> backCloudSprites;
    vector<ImageComponent*> backCloudImages;

    for (int i = 0; i < 4; i++){
        Sprite* sprite = new Sprite(&game);
        sprite->setScale(3);
        sprite->setPosition(Cloud::generate_random_number(0,game.getDisplayWidth()),Cloud::generate_random_number(0,game.getDisplayHeight() - 500));
        ImageComponent* image = new ImageComponent(sprite,2);
        image->setTexture(backClouds[0]);
        cout << "image dimensions: " << image->getTextureWidth();
        backCloudSprites.emplace_back(sprite);
        backCloudImages.emplace_back(image);
    }





    unsigned long framesSinceCutscene = 0;
    bool startedGame = false;
    while(game.isRunning()){
        system_timer.tick();
        accumulated_seconds += system_timer.elapsed_seconds;
        game.deltaTime = system_timer.elapsed_seconds;
        if (isgreater(accumulated_seconds, CYCLE_TIME)){
            if (musicSinceStart >= 8860){ // Ticks for euphoria to finish
                playMusic("src/assets/euphoria.wav",SDL_MIX_MAXVOLUME / 2);
                musicSinceStart = 0;
            }
            for (int i = 0; i < 4; i++){
                backCloudSprites[i]->changeX(1);
                if (backCloudSprites[i]->getPosition().x > game.getDisplayWidth()){
                    backCloudSprites[i]->setPosition(0 - backCloudImages[i]->getTextureWidth(),Cloud::generate_random_number(0,game.getDisplayHeight() - 500));
                }
            }

            if (startedGame){
                if (game.currentLevel == 19 && player->getPosition().x > 1000 && player->getPosition().x < 1100 && player->getPosition().y < 395){
                    framesSinceCutscene ++;
                    if (framesSinceCutscene > 30){
                        backgroundImage.setTexture(game.getTexture("src/assets/cookieMonster/sprite_1.png"));
                        ending.setPosition(3000,3000);
                    }
                    if (framesSinceCutscene > 60){
                        backgroundImage.setTexture(game.getTexture("src/assets/cookieMonster/sprite_2.png"));
                    }
                    if (framesSinceCutscene > 90){
                        backgroundImage.setTexture(game.getTexture("src/assets/cookieMonster/sprite_3.png"));
                    }
                    if (framesSinceCutscene > 150){
                        backgroundImage.setTexture(game.getTexture("src/assets/cookieMonster/sprite_4.png"));
                        player->setScale(0);
                    }
                    if (framesSinceCutscene > 200){
                        game.newLevel();
                    }
                }
                
                if (!game.sameLevel()){
                    for (int i = 0; i < 4; i++){
                        backCloudSprites[i]->setPosition(Cloud::generate_random_number(0,game.getDisplayWidth()),Cloud::generate_random_number(0,game.getDisplayHeight() - 500));
                    }
                    cout << "New Level. Now Level: " << game.getCurrentLevel() << endl;
                    if (game.getCurrentLevel() > 19){
                        break;
                    }
                    backgroundImage.setTexture(backgrounds[(game.getCurrentLevel()-1) / 2]);
                    if (game.currentLevel == 19){
                        backgroundImage.setTexture(game.getTexture("src/assets/cookieMonster/sprite_0.png"));
                        pauseAudio();
                        endingImage.setTexture(game.getTexture("src/assets/COOKIE.png"));
                        ending.setScale(3);
                        ending.setPosition(850, 351);
                    }
                    Cloud::destroyClouds();
                    game.clearData();
                    cout << "Destroyed Clouds Succesfully.." << endl;
                    cout << "CLEARED DATA!!!" << endl;
                    if (game.currentLevel != 19){
                        Cloud* cloud = new Cloud(&game,5,false);
                        Cloud::setupClouds(&game,game.amountOfClouds);
                    }
                    
                }
                //cout << "DeltaTime: " << game.getDeltaTime() << endl;

            }
            else{
                titleScreen.changePosition(0,sin(titleScreenHoverOffset)*0.5);
                titleScreenHoverOffset += 0.01;
                auto keys = game.getKeys();
                if (keys[SDL_SCANCODE_SPACE]){
                    startedGame = true;
                    player = new Player(&game);
                    Cloud::setupClouds(&game,game.amountOfClouds);
                    titleScreen.setScale(0);
                }


            }
            game.processInput();
            if(game.isRunning()){game.updateGame();}
            if(game.isRunning()){game.generateOutput();}
            accumulated_seconds = 0;

        }
    }
    if (game.currentLevel >= 19){
    }
    if (Cloud::clouds.size() > 0){
        Cloud::destroyClouds();
    }
    for (auto it = backCloudSprites.begin(); it < backCloudSprites.end(); ++it){
        cout << "Deleted Cloud #" << it - backCloudSprites.begin();
        delete *it;

    }
    player->setState(Player::Dead);
    game.clearData();
    endAudio();
    SDL_Quit();
    cout << "You Got To Level: " << game.level << endl;
    cout << "Size of clouds vector: " << Cloud::clouds.size() << endl;

}



/*
class mousePointer : public Sprite{
    public:
        ImageComponent* image;
        CollisionComponent* collider;
        mousePointer(Game *game): Sprite(game){
            image = new ImageComponent(this);
            collider = new CollisionComponent(this);
        }
        ~mousePointer(){
            delete image;
            delete collider;
        }
};


*/