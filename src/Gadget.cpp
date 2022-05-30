#include <iostream>

#include "Gadget.h"
#include "Cloud.h"
#include "frame/Game.h"
#include "frame/Sprite.h"

using namespace std;


Gadget::Gadget(Game *game,Cloud *cloudFather, int gadgetSlot):Sprite(game), cloudFather(cloudFather){
    cout << "Gadget Created." << endl;
    image = new ImageComponent(this);
    collider = new StaticColliderComponent(this);
    setScale(2);


    int weight = Cloud::generate_random_number(0,100);
    if (weight > 5){
        gadgetType = spike;
    }
    else{
        gadgetType = superJump;
    }

    //gadgetType = (GadgetType) Cloud::generate_random_number(0,1);
    cout << "GADGET TYPE: " << gadgetType;
    if (gadgetType == spike){
        setID(8);
        setHitbox(10,4,1,8);
        image->setTexture(game->getTexture("src/assets/spike.png"));
    }
    if (gadgetType == superJump){
        setID(9);
        setHitbox(12,12,0,0);
        image->setTexture(game->getTexture("src/assets/superJump.png"));
    }
    this->gadgetSlot = gadgetSlot;


}

void Gadget::update(float deltaTime){
    //cout << "IM UPDATING AND IM A GADGET" << endl;
    position.y = cloudFather->getPosition().y - image->getTextureHeight();
    position.x = cloudFather->getPosition().x + (gadgetSlot * image->getTextureWidth());
}