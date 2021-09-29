#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shader.h"
#include "texture.h"
#include "game_object.h"
// Czasteczki
struct Particle {
    glm::vec2 Position, Velocity;
    glm::vec4 Color;
    float     Life;
    Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};

class ParticleGenerator
{
public:
    ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount);

    // Fun
    void Update(float dt, GameObject& object, unsigned int newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
    void Draw();
private:
    // State
    std::vector<Particle> particles;
    unsigned int amount;
    // render
    void respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
    Shader shader;
    Texture2D texture;
    unsigned int VAO;

    // Fun
    void init();
    unsigned int firstUnusedParticle();
};

#endif

#pragma once
