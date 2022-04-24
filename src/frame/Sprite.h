#ifndef SPRITE_
#define SPRITE_
#include <vector>
#include "math.h"
#include "Game.h"

class Component{
public:
    Component(class Sprite* baseSprite, int updateOrder = 1000);
    virtual ~Component();
    virtual void update(float deltaTime);
    
    int getUpdateOrder() const {return updateOrder;}

protected:
    class Sprite* baseSprite;
    int updateOrder;

};

class ImageComponent : public Component{

public:
    ImageComponent(class Sprite *baseSprite, int drawOrder = 1000);
    ~ImageComponent();
    virtual void draw(SDL_Renderer *renderer);
    virtual void setTexture(SDL_Texture *texture);

    int getDrawOrder() const {return drawOrder;}
    int getTextureHeight() const {return drawOrder;}
    int getTextureWidth() const {return textureHeight;}


protected:
    SDL_Texture* texture;
    int drawOrder;
    int textureWidth;
    int textureHeight;
};




class Sprite{
public:
    enum State{
        Active,
        Frozen,
        Dead
    };

    Sprite(class Game *game);

    ~Sprite();
    Game* getGameInstance(){return gameInstance;}
    void update(float deltaTime);
    void updateComponents(float deltaTime);
    virtual void updateSprite(float deltaTime);
    State getState() {return state;}

    void addComponent(class Component *component);
    void removeComponent(class Component *component);
    void outputComponents();


    Vector2 getPosition(){return position;}
    void setPosition(Vector2 newPosition){position = newPosition;}
    float getScale(){return scale;}
    float getRotation(){return rotation;}
    void setRotation(float newRotation){rotation = newRotation;}
    

    // Add ways to add & remove components
protected:
    Vector2 position;
    float scale = 5;
    float rotation = 0; // radians
private:
    Game *gameInstance;
    State state;
    std::vector<class Component*> components;
};


#endif