#include "game.h"
#include "resource_manager.h"
#include "spriteRenderer.h"
#include "game_object.h"
#include "ball_object.h"
#include "particle_generator.h"
#include <iostream>

SpriteRenderer* Renderer;
BallObject* Player;
BallObject* SUN;
ParticleGenerator* Particles;

// const
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -1.0f);
const glm::vec2 ZERO(0.0f, 0.0f);
const float BALL_RADIUS = 20.5f;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{}

Game::~Game()
{
    delete Renderer;
    delete Player;
    delete SUN;
    delete Particles;
}

bool CheckCollision(BallObject& one, GameObject& two)
{
    glm::vec2 center(one.Position + one.Radius);
    glm::vec2 aabb_half_extents(two.Size.x / 2.0f, two.Size.y / 2.0f);
    glm::vec2 aabb_center(
        two.Position.x + aabb_half_extents.x,
        two.Position.y + aabb_half_extents.y
    );
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    glm::vec2 closest = aabb_center + clamped;
    difference = closest - center;
    return glm::length(difference) < one.Radius;
}


void Game::Init()
{
    // Loading to ResourceManager - storage

    ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.frag", nullptr, "sprite");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    Shader myShader;
    myShader = ResourceManager::GetShader("sprite");
    Renderer = new SpriteRenderer(myShader);
    ResourceManager::LoadTexture("textures/background.jpg", false, "background");
    ResourceManager::LoadTexture("textures/mines2.png", true, "face");
    ResourceManager::LoadTexture("textures/sun.png", true, "sun");
    ResourceManager::LoadTexture("textures/spacestation.png", true, "ship");
    ResourceManager::LoadShader("shaders/particle.vs", "shaders/particle.frag", nullptr, "particle");
    ResourceManager::LoadTexture("textures/particle.png", true, "particle");
    Particles = new ParticleGenerator(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 500);
    ResourceManager::GetShader("particle").Use().SetMatrix4("projection", projection);

    // Levels

    GameLevel one; one.Load("levels/one.txt");
    GameLevel two; two.Load("levels/two.txt");
    GameLevel three; three.Load("levels/three.txt");
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Level = 0;
    
    // Basic pos

    glm::vec2 POS = glm::vec2(this->Width / 2.0f - 60.0f, this->Height / 2.0f - 60.0f);
    SUN = new BallObject(POS, 60.0f, ZERO, ResourceManager::GetTexture("sun"));
    glm::vec2 playerPos = glm::vec2(this->Width / 2.0f, this->Height);
    Player = new BallObject(playerPos, 30.0f, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("ship"));
}

void Game::DoCollisions(float dt)
{
    for (BallObject& mine : this->Levels[this->Level].Bricks)
    {
        mine.Stuck = false;

        if (!mine.Destroyed)
        {
            mine.Move(dt, this->Width);
            Particles->Update(dt, mine, 2, glm::vec2(4.2f, 4.2f));
            if (CheckCollision(mine, *Player))
            {
                mine.Destroyed = true;
            }
        }
    }
}

void Game::Update(float dt)
{
    if (CheckCollision(*SUN, *Player)) {
        this->ResetLevel();
        this->ResetPlayer();
    }

    this->DoCollisions(dt);
    Particles->Update(dt, *Player, 2, glm::vec2(4.2f, 4.2f));
    if (this->Levels[this->Level].IsCompleted()){
        this->Level += 1;
        this->ResetLevel();
        this->ResetPlayer();
    }
}

void Game::ResetLevel()
{
    if (this->Level == 0)
        this->Levels[0].Load("levels/one.txt");
    else if (this->Level == 1)
        this->Levels[1].Load("levels/two.txt");
    else if (this->Level == 2)
        this->Levels[2].Load("levels/three.txt");
}

void Game::ResetPlayer()
{
    Player->Position = glm::vec2(this->Width / 2.0f, this->Height - 120.0f );
    Player->Velocity.x = 0; Player->Velocity.y = 0;
}



void Game::ProcessInput(float dt)
{
    if (this->State == GAME_ACTIVE)
    {
        if (this->Keys[GLFW_KEY_A])
        {
            if (Player->Position.x >= 0.0f)
            {
                Player->Velocity.x -= 0.1f;
            }
        }
        if (this->Keys[GLFW_KEY_D])
        {
            if (Player->Position.x <= this->Width - Player->Size.x)
            {
                Player->Velocity.x += 0.1f;
            }
        }
        if (this->Keys[GLFW_KEY_W])
        {
                Player->Velocity.y -= 0.1f;
        }
        if (this->Keys[GLFW_KEY_S])
        {
                Player->Velocity.y += 0.1f;
        }
        if (this->Keys[GLFW_KEY_SPACE])
        if (Player->Position.x < 700)Player->Velocity.x += 0.05f;
        if (Player->Position.x > 700)Player->Velocity.x -= 0.05f;
        if (Player->Position.y < 500)Player->Velocity.y += 0.05f;
        if (Player->Position.y > 500)Player->Velocity.y -= 0.05f;
        Player->Position += Player->Velocity * dt;
    }

}
void Game::Render() {
    if (this->State == GAME_ACTIVE)
    {
        Texture2D myTexture;
        myTexture = ResourceManager::GetTexture("background");
        Renderer->DrawSprite(myTexture, glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);
        Player->Draw(*Renderer);
        SUN->Draw(*Renderer);
        this->Levels[this->Level].Draw(*Renderer);
        Particles->Draw();
    }
}