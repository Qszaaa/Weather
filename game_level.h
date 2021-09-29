#pragma once
#ifndef GAMELEVEL_H
#define GAMELEVEL_H
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "game_object.h"
#include "ball_object.h"
#include "spriteRenderer.h"
#include "resource_manager.h"

class GameLevel
{
public:
    std::vector<BallObject> Bricks;
    GameLevel() { }
    void Load(const char* file);
    void Draw(SpriteRenderer& renderer);
    bool IsCompleted();
private:
    void init(std::vector<std::vector<unsigned int>> tileData);
};

#endif

