#ifndef GAME_H
#define GAME_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "game_level.h"

enum GameState {
    GAME_ACTIVE,
    GAME_MENU
};

class Game
{
public:
    Game(unsigned int width, unsigned int height);
    ~Game();

    GameState State;
    bool Keys[1024];
    unsigned int Width, Height;
    std::vector<GameLevel> Levels;
    unsigned int  Level;

    // FUN
    void Init();
    void DoCollisions(float dt);
    void ProcessInput(float dt);
    void Update(float dt);
    void ResetLevel();
    void ResetPlayer();
    void Render();
};

#endif
