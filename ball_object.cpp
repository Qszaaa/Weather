#include "ball_object.h"
#include <iostream>

BallObject::BallObject(): GameObject(), Radius(12.5f), Stuck(false) { }

BallObject::BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite): GameObject(pos, glm::vec2(radius * 2.0f, radius * 2.0f), sprite, glm::vec3(1.0f), velocity), Radius(radius), Stuck(true) { }

glm::vec2 BallObject::Move(float dt, unsigned int window_width)
{

    if (!this->Stuck)
    {
        if(this->Position.x < 700)this->Velocity.x += 0.05f;
        if(this->Position.x > 700)this->Velocity.x -= 0.05f;
        if(this->Position.y < 500)this->Velocity.y += 0.05f;
        if(this->Position.y > 500)this->Velocity.y -= 0.05f;
        this->Position += this->Velocity * dt;
    }
    return this->Position;
}

void BallObject::Reset(glm::vec2 position, glm::vec2 velocity)
{
    this->Position = position;
    this->Velocity = velocity;
    this->Stuck = true;
}

