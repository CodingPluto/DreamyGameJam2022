#ifndef SPRITE_
#define SPRITE_
#include <vector>
#include <array>
#include <unordered_map>
#include "math.h"
#include "Game.h"
#include "SDL.h"

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


class AnimationComponent : public ImageComponent{
public:
    AnimationComponent(class Sprite* baseSprite, int fps, bool looping, int updateOrder = 1000);
    ~AnimationComponent();
    void update(float deltaTime); //!
    void addAnimationTextures(const std::vector<SDL_Texture*> &textures, std::string animationName); //!
    void setAnimation(std::string animationName){currentAnimationTextures = animationName;}
    float getAnimationFPS(){return fps;}
    void setAnimationFPS(float newFps){fps = newFps;}
    void clearAnimation();
    void toggleLooping(bool newLooping){looping = newLooping;}
private:
    bool looping;
    bool updateAnimation = true;
    float fps;
    std::unordered_map<std::string, std::vector<SDL_Texture*>> mapAnimationTextures;
    std::string currentAnimationTextures;
    float currentFrame;

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
    void setPosition(float x, float y){setX(x); setY(y);}
    void changePosition(float x, float y){changeX(x); changeY(y);}
    void setX(float x){position.x = x;}
    void setY(float y){position.y = y;}
    void changeX(float x);
    void changeY(float y);
    float getScale(){return scale;}
    void setScale(float newScale){scale = newScale;}
    float getRotation(){return rotation;}
    void setRotation(float newRotation){rotation = newRotation;}


    // Add ways to add & remove components
protected:
    Vector2 position;
    float scale = 5;
    float rotation = 0; // radians
private:
    Game *gameInstance;
    State state = Active;
    std::vector<class Component*> components;
};


#endif