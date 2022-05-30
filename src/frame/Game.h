#ifndef GAME_
#define GAME_

#include "SDL.h"
#include "SDL_image.h"
#include <unordered_map>
#include "vector"
#include "Sprite.h"
#include "math.h"



namespace GameLoopFunctions{
    void processMainInput();
    void updateMainGame();
    void generateMainOutput();
};

class Game{
public:
    Game();
    ~Game();
    void initalize(const char title[], unsigned short xStart, unsigned short yStart, unsigned short width, unsigned short height, bool fullscreen);
    float getDeltaTime();
    void incrementTicks();
    void exitGame();
    bool isRunning();
    void outputDelta();
    void clearData();

    void AddImage(class ImageComponent *image);
    void RemoveImage(class ImageComponent *image);
    void AddSprite(class Sprite* sprite);
    void RemoveSprite(class Sprite* sprite);

    void outputSprites();
    void outputImageComponents();
    SDL_Texture* getTexture(const std::string& textureName);
    void processInput();
    void updateGame();
    void generateOutput();
    unsigned short getDisplayWidth(){return displayWidth;}
    unsigned short getDisplayHeight(){return displayHeight;}
    const Uint8* getKeys(){return keys;}
    double deltaTime = 0;


private:
    int displayWidth;
    int displayHeight;
    bool gameRunning = false;
    std::unordered_map<std::string,SDL_Texture*> textureMap;
    std::vector<class Sprite*> sprites;
    std::vector<class Sprite*> pendingSprites;
    std::vector<class ImageComponent*> imageComponents;
    bool updatingSprites = false;
    uint64_t ticksThisFrame = 0;

    const Uint8 *keys;
    SDL_Window *window;
    SDL_Renderer *renderer;
};

#endif