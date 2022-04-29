#include <iostream>
#include <vector>
#include "math.h"
#include "Sprite.h"
#include "Game.h"


using namespace std;


vector<Component*> CollisionComponent::collisionObjects;

void removeCollider(Component *colliderAddress){
    auto it = CollisionComponent::collisionObjects.begin();
    while(it < CollisionComponent::collisionObjects.end() && *it != colliderAddress){
        cout << "Address: " << colliderAddress << endl;
        it++;
    }
    cout << "Deleting Collider" << endl;
    if (it != CollisionComponent::collisionObjects.end()){
        CollisionComponent::collisionObjects.erase(it);
    }
    else{
        cout << ("Collision object has already been deleted.") << endl;
    }
}



StaticColliderComponent::StaticColliderComponent(Sprite* baseSprite, int updateOrder):Component(baseSprite,updateOrder){
    CollisionComponent::collisionObjects.emplace_back(this);
    cout << "Created collider object" << endl;
}

StaticColliderComponent::~StaticColliderComponent(){
    removeCollider(this);
    cout << "Destroyed Collider Object" << endl;
}


void sharedPhysicsFunction(Vector2 *velocity, Vector2 *position, float deltaTime, float offset){
    (*velocity).x *= 1/(1+deltaTime+offset);
    (*velocity).y *= 1/(1+deltaTime+offset);
    *position += *velocity;
}


CollisionComponent::CollisionComponent(Sprite* baseSprite, int updateOrder): Component(baseSprite,updateOrder){
    cout << "Collison object started to create!" << endl;
    collisionObjects.push_back(this);
    cout << "Collison object created!" << endl;
}
CollisionComponent::~CollisionComponent(){
    removeCollider(this);
}

void CollisionComponent::addCollidingObject(Component* collidingObject){
    collidingObjects.emplace_back(collidingObject);
}

bool CollisionComponent::checkCollision(Component *collider1, Component *collider2){
    float Pos1X = collider1->baseSprite->getPosition().x;
    float Pos1Y = collider1->baseSprite->getPosition().y;
    float length1 = collider1->baseSprite->getHitboxLength();
    float height1 = collider1->baseSprite->getHitboxHeight();
    float Pos2X = collider2->baseSprite->getPosition().x;
    float Pos2Y = collider2->baseSprite->getPosition().y;
    float length2 = collider2->baseSprite->getHitboxLength();
    float height2 = collider2->baseSprite->getHitboxHeight();

    if (Pos1X + length1 < Pos2X){
        return false;
    }
    if (Pos2X + length2 < Pos1X){
        return false;
    }
    if (Pos1Y + height1 < Pos2Y){
        return false;
    }
    if (Pos2Y + height2 < Pos1Y){
        return false;
    }

    cout << "Colliding \n\n\n\n\n\n\n\n" << endl;

    return true;
}





void CollisionComponent::update(float deltaTime){
    for (auto collider : collisionObjects){
        if (collider != this){
            
            colliding = checkCollision(this,collider);
            collidingObjects.clear();
            if (colliding){
                addCollidingObject(collider);
                cout << "Colliding Object 0" << collidingObjects[0] << endl;
            }
        }
    }
}

PhysicsComponent::PhysicsComponent(Sprite* baseSprite, int updateOrder):Component(baseSprite,updateOrder){
    cout << "Created Physics Component!" << endl;
    velocity.x = 0; velocity.y = 0;
    positionPtr = (baseSprite->getPositionRefrence());
}

PhysicsComponent::~PhysicsComponent(){}

void PhysicsComponent::update(float deltaTime){
    sharedPhysicsFunction(&velocity,positionPtr,deltaTime,friction);
}

AnimationComponent::AnimationComponent(Sprite* baseSprite, int fps, bool looping, int updateOrder): ImageComponent(baseSprite,updateOrder),fps(fps),looping(looping),currentFrame(0.0){
    cout << "Animation Component Created!" << endl;
    vector<SDL_Texture*> nullTextures;
    mapAnimationTextures["NONE"] = nullTextures;
}

void AnimationComponent::clearAnimation(){
    currentAnimationTextures = "NONE";
    texture = nullptr;
}


AnimationComponent::~AnimationComponent(){
    cout << "Animation Component Has Been Destroyed" << endl;
}

void AnimationComponent::update(float deltaTime){
    if (mapAnimationTextures[currentAnimationTextures].size() > 0){
        if (updateAnimation){
            currentFrame += fps * deltaTime;
        
        while(currentFrame >= mapAnimationTextures[currentAnimationTextures].size()){
            currentFrame -= mapAnimationTextures[currentAnimationTextures].size();
            if (!looping){
                updateAnimation = false;
            }
        }
        setTexture(mapAnimationTextures[currentAnimationTextures][(int)(currentFrame)]);
        }
    }
}

void AnimationComponent::addAnimationTextures(const std::vector<SDL_Texture*> &textures, string animationName){
    mapAnimationTextures[animationName] = textures;
}



void ImageComponent::draw(SDL_Renderer *renderer){
    if (texture){
        SDL_Rect rect;
        rect.w = (int)(textureWidth * baseSprite->getScale());
        rect.h = (int)(textureHeight * baseSprite->getScale());
        rect.x = (int)(baseSprite->getPosition().x - rect.w / 2);
        rect.y = (int)(baseSprite->getPosition().y - rect.h / 2);
        SDL_RenderCopyEx(renderer,texture,nullptr,&rect,-Math::ToDegrees(baseSprite->getRotation()),nullptr,SDL_FLIP_NONE);
    }
}

void ImageComponent::setTexture(SDL_Texture *texture){
    this->texture = texture;
    SDL_QueryTexture(texture,nullptr,nullptr,&textureWidth,&textureHeight);
}

ImageComponent::ImageComponent(Sprite *baseSprite, int drawOrder):Component(baseSprite),texture(nullptr),drawOrder(drawOrder),textureWidth(0),textureHeight(0){
    (baseSprite->getGameInstance())->AddImage(this);
}
ImageComponent::~ImageComponent(){
    (baseSprite->getGameInstance())->RemoveImage(this);
    cout << "Finishing destructor in ImageComponent" << endl;
}



Component::Component(Sprite *baseSprite, int updateOrder): baseSprite(baseSprite), updateOrder(updateOrder) {
    cout << "Started creating component" << endl;
    baseSprite->addComponent(this);
    cout << "Created component" << endl;
}
Component::~Component(){
    baseSprite->removeComponent(this);
    cout << "Removed Component" << endl;
}
void Component::update(float DeltaTime){}


Sprite::~Sprite(){
    cout << "Trying to remove sprite" << endl;
    gameInstance->RemoveSprite(this);
}

Sprite::Sprite(Game *game){
    gameInstance = game;
    game->AddSprite(this);
    cout << "Sprite created." << endl;
    
}

void Sprite::updateComponents(float deltaTime){
    for (auto it = components.begin(); it < components.end(); ++it){
        (*it)->update(deltaTime);
    }
}


void Sprite::update(){}

void Sprite::updateSprite(float deltaTime){
    if (state == Active){
        updateComponents(deltaTime);
    }
}

void Sprite::addComponent(Component* component){
    cout << "begin add components function" << endl;
    auto it = components.begin();
    while (it != components.end()){
        if (component->getUpdateOrder() < (*it)->getUpdateOrder()){
            break;
        }
        ++it;
    }
    components.insert(it,component);
}

void Sprite::removeComponent(Component* component){
    auto it = components.begin();
    while (*it != component && it != components.end()){++it;}
    if (it != components.end()){
        components.erase(it);
    }
}

void Sprite::outputComponents(){
    for (auto it = components.begin(); it < components.end(); ++it){
        cout << *it << endl;
    }
}
void Sprite::changeY(float y){
    position.y += y * gameInstance->getDeltaTime();
}
void Sprite::changeX(float x){position.x += x * gameInstance->getDeltaTime();}