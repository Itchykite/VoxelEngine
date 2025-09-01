#pragma once

#include "../../include/glad/include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <memory>
#include <stdexcept>

class Game 
{
public: 
    Game();
    ~Game();
    void run();

private: 
    void init();
    void loop();
    void cleanup();

private:
    GLFWwindow* window = nullptr;
    std::unique_ptr<class Renderer> renderer;
    std::unique_ptr<class Camera> camera;
};

// ------------------------------- Callback Functions -------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
