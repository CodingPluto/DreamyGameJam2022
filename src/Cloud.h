#ifndef CLOUD_
#define CLOUD_
#include <vector>
#include "frame/Sprite.h"
#include "Player.h"



class Cloud : public Sprite{
protected:

private:
    float hoverSpeed = 0.01;
    float hoverRange = 0.3;
    bool hovering = false;
    void update(float deltaTime);
    float hoverOffset = 0;

    float windSpeed;
    int cloudIndex;
    bool customCloud = false;

    LucyGame *gameInstance;

    std::vector<Sprite*> gadgets;
    void setupGadgets();
    void destroyGadgets();


public:
    int getWindSpeed(){return windSpeed;}
    short cloudLength;
    ImageComponent* image;
    PhysicsComponent* physics;
    StaticColliderComponent* collider;
    Cloud(LucyGame *game, int cloudIndex);
    Cloud(LucyGame *game, int cloudLength, bool gadgets);
    static int generate_random_number(int min, int max);

    ~Cloud();
    static bool rngSetup;
    static std::vector<Cloud*> clouds;
    static std::vector<Vector2> positionsTaken;
    static void setupClouds(LucyGame *game, int amountOfClouds);
    static void destroyClouds();
    static std::vector<std::vector<SDL_Texture*>> cloudSprites;
    static LucyGame *gameReference;
    static void setupCloudImages();
    

};

#endif
