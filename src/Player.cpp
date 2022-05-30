#include <iostream>
#include "frame/Sprite.h"
#include "frame/Game.h"
#include "Player.h"
#include "frame/audio.h"
using namespace std;




void Player::update(float deltaTime){

    auto keys = gameInstance->getKeys();
    float gravityStrength = 1.2;
    float movementSpeed = 1;
    bool running = false;
    bool charging = false;
    bool touchingCloud = false;
    int jumpStrength = 20;




    if (position.y < 0 - animation->getTextureHeight() || keys[SDL_SCANCODE_0]){
        gameInstance->newLevel();
        setPosition(position.x,gameInstance->getDisplayHeight() - animation->getTextureHeight());
        cout << "NEW LEVEL!!" << endl;
    }
    if (endCutscene){
        if (gameInstance->currentLevel > 19){
            endCutscene = false;
        }
        physics->setVelocity(0,0);
        if (position.y > 390){
            position.y -= 3;
        }
        if (position.x < 1050){
            position.x += 3;
        }
        if (position.x > 1050){
            position.x -= 3;
        }
    }
    else{
        physics->changeVelocityY(gravityStrength);
    }
    framesSinceCloudTouched += 1;
    if (position.y > gameInstance->getDisplayHeight() - animation->getTextureHeight()){
        lastCloudTouched = nullptr;
        setPosition(position.x, gameInstance->getDisplayHeight() - animation->getTextureHeight());
        touchingCloud = true;
        animation->toggleLooping(true);
        framesSinceCloudTouched = 0;
    }
    if (position.x < 0){
        position.x = 0;
    }
    if (position.x > gameInstance->getDisplayWidth() - animation->getTextureWidth()){
        position.x = gameInstance->getDisplayWidth() - animation->getTextureWidth();
    }



    vector<Component*> objectsTouched = collider->getCollidingObjects();
    if ((objectsTouched.size() > 0)){
        Component* cloudTouched = nullptr;
        Component* spikeTouched = nullptr;
        Component* superJumpTouched = nullptr;

        for (auto it = objectsTouched.begin(); it < objectsTouched.end(); ++it){
            if ((*it)->baseSprite->getID() == 5){
                cloudTouched = (*it);
                //break;
            }
            else if ((*it)->baseSprite->getID() == 8){
                spikeTouched = (*it);
            }
            else if ((*it)->baseSprite->getID() == 9){
                playSound("src/assets/superjump.wav", SDL_MIX_MAXVOLUME);
                superJumpTouched = (*it);
            }
        }

        if (spikeTouched != nullptr){
            dead = true;
        }
        else if (superJumpTouched != nullptr){
            charged = true;
            chargeAmount = 61;
        }
        if (cloudTouched != nullptr && !inAir){
            lastCloudTouched = cloudTouched;
            setPosition(position.x,(cloudTouched->baseSprite->getPosition().y)- animation->getTextureHeight() + cloudTouched->baseSprite->getHitboxOffsetY());
            physics->setVelocityY(0);
            touchingCloud = true;
            animation->toggleLooping(true);
            framesSinceCloudTouched = 0;
        }

    }

    /*if(((keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN]) && !charged)){
        charging = true;
        animation->setAnimationFPS(2);
        chargeAmount++;
        cout << "Charge Amount: " << chargeAmount << endl; 
    }*/
    if (framesSinceCloudTouched < 3 && lastCloudTouched != nullptr){changePosition(lastCloudTouched->baseSprite->movementSpeed,0);}
    if (!dead){
        if (chargeAmount > 60){
            charged = true;
        }
        if (charged && chargeAmount > 60){
            jumpStrength = 35;
            jumping = true;
            chargeAmount = 0;
        }
        if (!charging && (!charged || !jumping) && !endCutscene){
            if(keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP]){
                if (!(gameInstance->currentLevel == 19)){
                    if (touchingCloud){
                        jumping = true;
                    }
                }
                else{
                    endCutscene = true;
                }
            }
            if(keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]){
                physics->changeVelocityX(movementSpeed);
                running = true;
                animation->setFlip(false);
            }
            if(keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT]){
                physics->changeVelocityX(-movementSpeed);
                running = true;
                animation->setFlip(true);
            }
        }
        if (!charging && !inAir){
            animation->setAnimationFPS(10);
        }
        if (charged){
            animation->setAnimation("repluse");
        }
        if (jumping){
            if (!charged){
                animation->toggleLooping(false);
                animation->setAnimationFPS(2.5);
            }
            animation->setAnimation("jumping");
            touchingCloud = false;
            physics->setVelocityY(-jumpStrength);
            jumping = false;
            inAir = true;
            physics->setFrictionY(0.01);
        }
        else if (charging && !inAir){
            animation->setAnimation("charging");
        }
        else if (!inAir && framesSinceCloudTouched > 3){
            animation->setAnimation("falling");
        }
        else if (running && !inAir && framesSinceCloudTouched < 3){
            animation->setAnimation("running");
        }
        else if (framesSinceCloudTouched < 3 && !inAir){
            animation->setAnimation("idle");
        }
        else if (inAir && physics->getVelocityY() > -0.05){
            inAir = false;
            charged = false;
            animation->setAnimationFPS(10);
            physics->setFrictionY(0.2);
            animation->toggleLooping(true);
        }
    }
    else if (ticksSinceDead == 0){
        playSound("src/assets/death.wav", SDL_MIX_MAXVOLUME);
        animation->toggleLooping(false);
        animation->setAnimation("die");
        animation->setAnimationFPS(8);
        physics->setVelocity(0,0);
        charged = false;
        charging = false;
        inAir = false;
        framesSinceCloudTouched = 0;
    }
    if (dead){
        ticksSinceDead ++;
    }
    if (ticksSinceDead > 30){
        animation->toggleLooping(true);
        dead = false;
        ticksSinceDead = 0;
        setPosition(0,gameInstance->getDisplayHeight() - animation->getTextureHeight());
        physics->setVelocity(0,0);
    }
}








Player::Player(LucyGame *game): Sprite(game){
    gameInstance = game;
    setScale(2);
    animation = new AnimationComponent(this,10,true,100000);
    collider = new CollisionComponent(this);
    physics = new PhysicsComponent(this);
    string animationsPath = "src/assets/lucyAnimations/";
    vector<SDL_Texture*> idle = {
        game->getTexture(animationsPath + "lucy_idle/" + "sprite_0.png"),
        game->getTexture(animationsPath + "lucy_idle/" + "sprite_1.png"),
        game->getTexture(animationsPath + "lucy_idle/" + "sprite_2.png"),
        game->getTexture(animationsPath + "lucy_idle/" + "sprite_3.png"),
        game->getTexture(animationsPath + "lucy_idle/" + "sprite_4.png"),
        game->getTexture(animationsPath + "lucy_idle/" + "sprite_5.png"),
        game->getTexture(animationsPath + "lucy_idle/" + "sprite_6.png"),
        game->getTexture(animationsPath + "lucy_idle/" + "sprite_7.png"),
        game->getTexture(animationsPath + "lucy_idle/" + "sprite_8.png"),
    };
    vector<SDL_Texture*> running = {
        game->getTexture(animationsPath + "lucy_running/" + "sprite_0.png"),
        game->getTexture(animationsPath + "lucy_running/" + "sprite_1.png"),
        game->getTexture(animationsPath + "lucy_running/" + "sprite_2.png"),
        game->getTexture(animationsPath + "lucy_running/" + "sprite_3.png"),
        game->getTexture(animationsPath + "lucy_running/" + "sprite_4.png"),
        game->getTexture(animationsPath + "lucy_running/" + "sprite_5.png"),
        game->getTexture(animationsPath + "lucy_running/" + "sprite_6.png"),
        game->getTexture(animationsPath + "lucy_running/" + "sprite_7.png"),
    };
    vector<SDL_Texture*> jumping = {
        game->getTexture(animationsPath + "lucy_Jump/" + "sprite_0.png"),
        game->getTexture(animationsPath + "lucy_Jump/" + "sprite_1.png"),
        game->getTexture(animationsPath + "lucy_Jump/" + "sprite_2.png"),
        game->getTexture(animationsPath + "lucy_Jump/" + "sprite_3.png"),
        game->getTexture(animationsPath + "lucy_Jump/" + "sprite_4.png"),
        game->getTexture(animationsPath + "lucy_Jump/" + "sprite_5.png"),
        game->getTexture(animationsPath + "lucy_Jump/" + "sprite_6.png"),
        game->getTexture(animationsPath + "lucy_Jump/" + "sprite_7.png"),
        game->getTexture(animationsPath + "lucy_Jump/" + "sprite_8.png"),
    };
    vector<SDL_Texture*> falling = {
        game->getTexture(animationsPath + "lucy_fall/" + "sprite_0.png"),
        game->getTexture(animationsPath + "lucy_fall/" + "sprite_1.png"),
        game->getTexture(animationsPath + "lucy_fall/" + "sprite_2.png"),
        game->getTexture(animationsPath + "lucy_fall/" + "sprite_3.png"),
    };
    vector<SDL_Texture*> charging = {
        game->getTexture(animationsPath + "lucy_charging/" + "sprite_0.png"),
        game->getTexture(animationsPath + "lucy_charging/" + "sprite_1.png"),
        game->getTexture(animationsPath + "lucy_charging/" + "sprite_2.png"),
        game->getTexture(animationsPath + "lucy_charging/" + "sprite_3.png"),
        game->getTexture(animationsPath + "lucy_charging/" + "sprite_4.png"),
        game->getTexture(animationsPath + "lucy_charging/" + "sprite_5.png"),
    };
    vector<SDL_Texture*> charged = {
        game->getTexture(animationsPath + "lucy_charged/" + "sprite_0.png"),
        game->getTexture(animationsPath + "lucy_charged/" + "sprite_1.png"),
        game->getTexture(animationsPath + "lucy_charged/" + "sprite_2.png"),
        game->getTexture(animationsPath + "lucy_charged/" + "sprite_3.png"),
        game->getTexture(animationsPath + "lucy_charged/" + "sprite_4.png"),
    };
    vector<SDL_Texture*> repluse = {
        game->getTexture(animationsPath + "lucy_repulse/" + "sprite_0.png"),
        game->getTexture(animationsPath + "lucy_repulse/" + "sprite_1.png"),
        game->getTexture(animationsPath + "lucy_repulse/" + "sprite_2.png"),
        game->getTexture(animationsPath + "lucy_repulse/" + "sprite_3.png"),
        game->getTexture(animationsPath + "lucy_repulse/" + "sprite_4.png"),
        game->getTexture(animationsPath + "lucy_repulse/" + "sprite_5.png"),
        game->getTexture(animationsPath + "lucy_repulse/" + "sprite_6.png"),
    };
    vector<SDL_Texture*> die = {
        game->getTexture(animationsPath + "lucy_die/" + "sprite_0.png"),
        game->getTexture(animationsPath + "lucy_die/" + "sprite_1.png"),
        game->getTexture(animationsPath + "lucy_die/" + "sprite_2.png"),
        game->getTexture(animationsPath + "lucy_die/" + "sprite_3.png"),
        game->getTexture(animationsPath + "lucy_die/" + "sprite_4.png"),
        game->getTexture(animationsPath + "lucy_die/" + "sprite_5.png"),
        game->getTexture(animationsPath + "lucy_die/" + "sprite_6.png"),
        game->getTexture(animationsPath + "lucy_die/" + "sprite_7.png"),
    };
    animation->addAnimationTextures(idle,"idle");
    animation->addAnimationTextures(running,"running");
    animation->addAnimationTextures(jumping,"jumping");
    animation->addAnimationTextures(falling,"falling");
    animation->addAnimationTextures(charging,"charging");
    animation->addAnimationTextures(charged,"charged");
    animation->addAnimationTextures(repluse,"repluse");
    animation->addAnimationTextures(die,"die");
    physics->setFriction(0.2,0.2);
    setHitbox(12,3,0,21);
    position.x = 0;
    position.y = game->getDisplayHeight() - animation->getTextureHeight();
}