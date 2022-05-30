#include <iostream>
#include <unordered_map>
#include <vector>
#include <ctime>
#include "frame/Sprite.h"
#include "Cloud.h"
#include "Gadget.h"
#include "Player.h"


using namespace std;


LucyGame *Cloud::gameReference;

vector<vector<SDL_Texture*>> Cloud::cloudSprites{};


void Cloud::setupCloudImages(){
    cloudSprites.emplace_back(vector<SDL_Texture*>{Cloud::gameReference->getTexture("src/assets/cloud1/sprite_0.png"),Cloud::gameReference->getTexture("src/assets/cloud1/sprite_1.png"),Cloud::gameReference->getTexture("src/assets/cloud1/sprite_2.png")});
    cloudSprites.emplace_back(vector<SDL_Texture*>{Cloud::gameReference->getTexture("src/assets/cloud2/sprite_0.png"),Cloud::gameReference->getTexture("src/assets/cloud2/sprite_1.png"),Cloud::gameReference->getTexture("src/assets/cloud2/sprite_2.png")});
    cloudSprites.emplace_back(vector<SDL_Texture*>{Cloud::gameReference->getTexture("src/assets/cloud3/sprite_0.png"),Cloud::gameReference->getTexture("src/assets/cloud3/sprite_1.png"),Cloud::gameReference->getTexture("src/assets/cloud3/sprite_2.png")});
    cloudSprites.emplace_back(vector<SDL_Texture*>{Cloud::gameReference->getTexture("src/assets/cloud4/sprite_0.png"),Cloud::gameReference->getTexture("src/assets/cloud4/sprite_1.png"),Cloud::gameReference->getTexture("src/assets/cloud4/sprite_2.png")});
    cloudSprites.emplace_back(vector<SDL_Texture*>{Cloud::gameReference->getTexture("src/assets/cloud5/sprite_0.png"),Cloud::gameReference->getTexture("src/assets/cloud5/sprite_1.png"),Cloud::gameReference->getTexture("src/assets/cloud5/sprite_2.png")});
}


bool Cloud::rngSetup = false; 
void setup_random_number_generation(){
    srand(time(0));
}
int Cloud::generate_random_number(int min, int max){
    if (!Cloud::rngSetup){
        setup_random_number_generation();
        Cloud::rngSetup = true;
    }
    if (max < min){
        cout << "num generation error: [max is more than min]" << endl;
        exit(1);
    }
    else{
        int offset = 0;

        if (min < 0){
            max += abs(min);
            offset += abs(min);
            min = 0;
        }

        return rand()%(max + 1 - min) + min - offset;

    }
}


vector<Cloud*> Cloud::clouds;
vector<Vector2> Cloud::positionsTaken;
void Cloud::setupClouds(LucyGame *game, int amountOfClouds){

    for (int i = 0; i < amountOfClouds; ++i){
        Cloud *cloud = new Cloud(game,i);
        cloud->setState(Active);
        clouds.emplace_back(cloud);
    }
}
void Cloud::destroyClouds(){
    for (auto it = clouds.begin(); it < clouds.end(); ++it){
        cout << "Cloud #" << it - clouds.begin() << " is being deleted." << endl;
        (*it)->~Cloud();
        (*it)->setState(Dead);
    }
    clouds.clear();
}









void Cloud::setupGadgets(){

    if (gameInstance->spikeRarity <= 2){
        gameInstance->spikeRarity = 2;
    }
    cout << "Started to setup Gadgets" << endl;
    for (int i = 0; i < cloudLength; ++i){
        if (generate_random_number(1,gameInstance->spikeRarity) == 1){
            Gadget* gadget = new Gadget(gameInstance,this,i);
            gadgets.emplace_back(gadget);
        }
    }
}

void Cloud::destroyGadgets(){
    for (auto it = gadgets.begin(); it < gadgets.end(); ++it){
        cout << "Set Gadget to dead!" << endl;
        (*it)->setState(Dead);
    }
}


Cloud::Cloud(LucyGame *game, int cloudLength, bool gadgets): Sprite(game){
    clouds.emplace_back(this);
    customCloud = true;
    gameInstance = game;
    setID(5);
    setScale(2);
    image = new ImageComponent(this);
    physics = new PhysicsComponent(this);
    collider = new StaticColliderComponent(this);
    image->setTexture(game->getTexture("src/assets/cloud5/sprite_2.png"));
    this->cloudLength = cloudLength;
    setHitbox(12 * cloudLength,3,0,1);
    position.x = game->getDisplayWidth()/2 - image->getTextureWidth()/2;
    position.y = game->getDisplayHeight() - 110;
    physics->setFriction(0.005,0.005);
    hoverSpeed = generate_random_number(1,5) / (float) generate_random_number(50,100);
    hoverRange = generate_random_number(1,3) / (float) 10;

    windSpeed = generate_random_number(20,50) / 20;
    movementSpeed = windSpeed;
    if (gadgets){
        setupGadgets();
    }

}



Cloud::Cloud(LucyGame *game, int cloudIndex):Sprite(game){
    gameInstance = game;
    setID(5);
    setScale(2);
    cout << "Cloud Created with instance " << this << endl;
    image = new ImageComponent(this);
    physics = new PhysicsComponent(this);
    collider = new StaticColliderComponent(this);
    cout << "Finished Creating Cloud components " << endl;

    cloudLength = generate_random_number(1,5);
    cout << "CLOUD LENGTH: " << cloudLength << endl;
    image->setTexture(cloudSprites[cloudLength - 1][generate_random_number(0,2)]);
    setHitbox(12 * cloudLength,3,0,1);
    cout << "Setting position of cloud " << endl;

    float potentialPositionX = generate_random_number(0,game->getDisplayWidth() - image->getTextureWidth());
    float potentialPositionY = generate_random_number(0,game->getDisplayHeight() - 100 - image->getTextureHeight());
    position.x = potentialPositionX;
    position.y = potentialPositionY;
    //positionsTaken.emplace_back(position);

    cout << "Position: " << position.x << " : " << position.y << endl;

    physics->setFriction(0.005,0.005);

    hoverSpeed = generate_random_number(1,5) / (float) generate_random_number(50,100);
    hoverRange = generate_random_number(1,3) / (float) 10;
    cout << "Done HoverSpeed" << endl;

    int stationary = generate_random_number(0,100);
    if (stationary > gameInstance->stationaryChance){
        windSpeed = 0;
    }
    else{
        windSpeed = generate_random_number(10,70) / 20;
    }
    movementSpeed = windSpeed;
    cout << "Done WindSpeed" << endl;
    setupGadgets();

}

Cloud::~Cloud(){
    destroyGadgets();
}

void Cloud::update(float deltaTime){
    changePosition(windSpeed,sin(hoverOffset)*hoverRange);
    hoverOffset += hoverSpeed;

    if (position.x > gameInstance->getDisplayWidth()){
        position.x = 0 - image->getTextureWidth();
        if (!customCloud){
            position.y = generate_random_number(0,gameInstance->getDisplayHeight() - 100 - image->getTextureHeight());
            movementSpeed = windSpeed;
        }
    }
    if (position.y > gameInstance->getDisplayHeight()){
        //Delete
    }



}

