#ifndef PLAYER_
#define PLAYER_

#include "frame/Sprite.h"
#include "frame/Game.h"


class LucyGame : public Game{
private:
public:
    int level;
    int currentLevel;
    int spikeRarity;
    int stationaryChance;
    LucyGame():Game(){
        level = 1;
        currentLevel = 1;
        amountOfClouds = 100;
        spikeRarity = 100;
        stationaryChance = 20;
    }
    int getCurrentLevel(){
        return level;
    }
    bool sameLevel(){
        if (level == currentLevel){
            return true;
        }
        else{
            currentLevel = level;
            return false;
        }
        return false;
    }
    void newLevel(){
        level++;
        std::cout << "It is now level: " << level << std::endl;
        std::cout << "Current Level is: " << currentLevel << std::endl;
        if (amountOfClouds > 35){
            amountOfClouds -= 3;
        }
        spikeRarity *= 0.8;
        stationaryChance += 5;
    }
    int amountOfClouds;

};



class Player : public Sprite{
public:
    AnimationComponent* animation;
    CollisionComponent* collider;
    PhysicsComponent* physics;
    Player(LucyGame *game);
    //~Player();
    void update(float deltaTime);
private:
    Component* lastCloudTouched;
    LucyGame* gameInstance;
    int chargeAmount = 0;
    bool endCutscene = false;
    bool charged = false;
    bool jumping = false;
    bool inAir = false;
    long framesSinceCloudTouched = 99;
    bool dead = false;
    int ticksSinceDead = 0;
};

#endif