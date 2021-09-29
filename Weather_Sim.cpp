#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "game.h"
#include "resource_manager.h"
#include <iostream>


// Buffery i obsluga klawiszy
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// width
const unsigned int SCREEN_WIDTH = 1400;
// height
const unsigned int SCREEN_HEIGHT = 1080;

Game Weather(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, char* argv[])
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, false);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Weather", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Glad oraz ERR
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glfwSetKeyCallback(window, key_callback);

    //config OpenGL
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Init
    Weather.Init();

    // Time
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // Petla
    while (!glfwWindowShouldClose(window))
    {
        // delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        // Obsluga input
        Weather.ProcessInput(deltaTime);

        // Update i time
        Weather.Update(deltaTime);

        // render
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        Weather.Render();

        // Buffery
        glfwSwapBuffers(window);
    }
    ResourceManager::Clear();
    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
    // Wyjscie
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (key >= 0 && key < 1024){
        if (action == GLFW_PRESS)
            Weather.Keys[key] = true;
        else if (action == GLFW_RELEASE)
            Weather.Keys[key] = false;
    }
}
