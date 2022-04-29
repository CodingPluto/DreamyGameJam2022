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

    class Sprite* baseSprite;
protected:
    int updateOrder;

};

class ImageComponent : public Component{

public:
    ImageComponent(class Sprite *baseSprite, int drawOrder = 1000);
    ~ImageComponent();
    virtual void draw(SDL_Renderer *renderer);
    virtual void setTexture(SDL_Texture *texture);

    int getDrawOrder() const {return drawOrder;}
    int getTextureHeight() const {return textureHeight;}
    int getTextureWidth() const {return textureWidth;}


protected:
    SDL_Texture* texture;
    int drawOrder;
    int textureWidth;
    int textureHeight;
};


class StaticColliderComponent : public Component{
    public:
        StaticColliderComponent(class Sprite* baseSprite, int updateOrder = 1000);
        ~StaticColliderComponent();
        void update(float deltaTime){}


    //StaticColliderComponent()
};

class PhysicsComponent : public Component{
    public:
        PhysicsComponent(class Sprite* baseSprite, int updateOrder = 1000);
        ~PhysicsComponent();
        void setVelocity(float velX, float velY){setVelocityX(velX); setVelocityY(velY);}
        void setVelocity(Vector2 newVelocity){velocity = newVelocity;}
        void setVelocityX(float velX){velocity.x = velX;}
        void setVelocityY(float velY){velocity.y = velY;}
        Vector2 getVelocity(){return velocity;}
        float getVelocityX(){return velocity.x;}
        float getVelocityY(){return velocity.y;}
        void changeVelocity(float velX, float velY){changeVelocityX(velX); changeVelocityX(velY);}
        void changeVelocityX(float velX){velocity.x += velX;}
        void changeVelocityY(float velY){velocity.y += velY;}
        void changeVelocity(Vector2 changeVelocity){velocity += changeVelocity;}
        void setFriction(float newFriction){friction = newFriction;}
        float getFriction(){return friction;}



        void update(float deltaTime);
        
        
    protected:
        Vector2 velocity;
        Vector2 *positionPtr;
    private:
        float friction = 0.1;

};

class CollisionComponent : public Component{
    public:
        CollisionComponent(class Sprite* baseSprite, int updateOrder = 1000);
        ~CollisionComponent();
        bool colliding = false;
        void touchPlane();
        void update(float deltaTime);
        static std::vector<Component*> collisionObjects;
        bool checkCollision(Component* collider1,Component* collider2);
        std::vector<Component*> getCollidingObjects(){return collidingObjects;}
        void addCollidingObject(Component* collidingObject);
        float getHitboxLength(){return hitbox.x;}
        float getHitboxHeight(){return hitbox.y;}
        void setHitbox(float length, float height){hitbox.x = length; hitbox.y = height;}
        Vector2 hitbox{0,0};
    private:
        std::vector<Component*> collidingObjects;
    
};








class AnimationComponent : public ImageComponent{
public:
    AnimationComponent(class Sprite* baseSprite, int fps, bool looping, int drawOrder = 1000);
    ~AnimationComponent();
    void update(float deltaTime); //!
    void addAnimationTextures(const std::vector<SDL_Texture*> &textures, std::string animationName); //!
    void setAnimation(std::string animationName){currentAnimationTextures = animationName;}
    float getAnimationFPS(){return fps;}
    void setAnimationFPS(float newFps){fps = newFps;}
    void clearAnimation();
    void toggleLooping(bool newLooping){looping = newLooping; updateAnimation = true;}
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
    virtual void update();
    void updateComponents(float deltaTime);
    virtual void updateSprite(float deltaTime);
    State getState() {return state;}

    void addComponent(class Component *component);
    void removeComponent(class Component *component);
    void outputComponents();


    Vector2 getPosition(){return position;}
    Vector2* getPositionRefrence(){return &position;}
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

    void setHitbox(float newLength, float newHeight){hitboxLength = newLength; hitboxHeight = newHeight;}
    float getHitboxLength(){return hitboxLength;}
    float getHitboxHeight(){return hitboxHeight;}


    // Add ways to add & remove components
protected:
    Vector2 position;
    float scale = 5;
    float rotation = 0; // radians
private:
    Game *gameInstance;
    float hitboxLength = 0;
    float hitboxHeight = 0;
    State state = Active;
    std::vector<class Component*> components;
};


#endif