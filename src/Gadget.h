#ifndef GADGET_
#define GADGET_

#include "frame/Game.h"
#include "frame/Sprite.h"
#include "Cloud.h"

class Gadget : public Sprite{
public:
    ImageComponent* image;
    StaticColliderComponent* collider;
    Gadget(Game *game,Cloud *cloudFather, int gadgetSlot);
    ~Gadget(){
        std::cout << "Gadget is being deleted.." << std::endl;
        if (image == nullptr){
            cout << "MEMORY LEAK!!!" << endl;
        }
        delete image;
        delete collider;
    }

private:
    int gadgetSlot;
    enum GadgetType{spike, superJump, bomb};
    GadgetType gadgetType;
    void update(float deltaTime);
    Cloud *cloudFather;
    

};
#endif