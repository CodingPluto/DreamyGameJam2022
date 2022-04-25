#include <iostream>
#include <vector>
#include "math.h"
#include "Sprite.h"
#include "Game.h"


using namespace std;




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
        }
        while(currentFrame >= mapAnimationTextures[currentAnimationTextures].size()){
            if (looping){
                currentFrame -= mapAnimationTextures[currentAnimationTextures].size();
            }
            else{
                currentFrame -= mapAnimationTextures[currentAnimationTextures].size();
                updateAnimation = false;
            }
        }
        setTexture(mapAnimationTextures[currentAnimationTextures][(int)(currentFrame)]);
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
        /*
        cout << "Width: " << rect.w << endl;
        cout << "Height: " << rect.h << endl;
        cout << "X: " << rect.x << endl;
        cout << "Y: " << rect.y << endl;*/
        SDL_RenderCopyEx(renderer,texture,nullptr,&rect,-Math::ToDegrees(baseSprite->getRotation()),nullptr,SDL_FLIP_NONE);
    }
}

void ImageComponent::setTexture(SDL_Texture *texture){
    this->texture = texture;
    SDL_QueryTexture(texture,nullptr,nullptr,&textureWidth,&textureHeight); 
    //cout << "New texture size{  Width: " << textureWidth << "  |  Height: " << textureHeight << "  }" << endl;
}

ImageComponent::ImageComponent(Sprite *baseSprite, int drawOrder):Component(baseSprite),texture(nullptr),drawOrder(drawOrder),textureWidth(0),textureHeight(0){
    (baseSprite->getGameInstance())->AddImage(this);
}
ImageComponent::~ImageComponent(){
    (baseSprite->getGameInstance())->RemoveImage(this);
    cout << "Finishing destructor in ImageComponent" << endl;
}



Component::Component(Sprite *baseSprite, int updateOrder): baseSprite(baseSprite), updateOrder(updateOrder) {
    baseSprite->addComponent(this);
}
Component::~Component(){
    baseSprite->removeComponent(this);
    cout << "Removed Component" << endl;
}
void Component::update(float deltaTime){}



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


void Sprite::updateSprite(float deltaTime){
    if (state == Active){
        updateComponents(deltaTime);
    }
}

void Sprite::addComponent(Component* component){
    auto it = components.begin();
    while (it != components.end()){
        if (component->getUpdateOrder() < (*it)->getUpdateOrder()){
            break;
        }
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