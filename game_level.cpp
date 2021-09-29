#include "game_level.h"

#include <fstream>
#include <sstream>
#include <iostream>


void GameLevel::Load(const char* file)
{
    // Clear

    this->Bricks.clear();

    // Get lvl from file

    unsigned int info;
    std::string line;
    std::ifstream fstream(file);
    std::vector<std::vector<unsigned int>> tileData;
    if(fstream)
    {
        while (std::getline(fstream, line)) 
        {
            std::istringstream sstream(line);
            std::vector<unsigned int> row;
            while (sstream >> info)
                row.push_back(info);
            tileData.push_back(row);
        }
        if (tileData.size() > 0)
            this->init(tileData);
    }
}

void GameLevel::Draw(SpriteRenderer& renderer)
{
    for (BallObject& tile : this->Bricks) {
        if (!tile.Destroyed)tile.Draw(renderer);
    }
}

bool GameLevel::IsCompleted()
{
    for (BallObject& tile : this->Bricks)
        if (!tile.Destroyed)
            return false;
    return true;
}

void GameLevel::init(std::vector<std::vector<unsigned int>> tileData)
{
    for (int i = 0; i < tileData.size() ; i++){
        std::cout << tileData[i][0] << std::endl;
        glm::vec2 POS = glm::vec2(tileData[i][0], tileData[i][1]);
        glm::vec2 VELOCITY(tileData[i][3], tileData[i][4]);
        this->Bricks.push_back(BallObject(POS, tileData[i][2], VELOCITY, ResourceManager::GetTexture("face")));
    }
}

