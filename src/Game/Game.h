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

public:
    GLFWwindow* window = nullptr;
    double windowWidth = 1920;
    double windowHeight = 1080;
    
    bool cameraControlEnabled = false;
    void processKeyboardInput(float deltaTime = 0.0f);

private: 
    void init();
    void loop();
    void cleanup();

private:
    std::unique_ptr<class Renderer> renderer;
    std::unique_ptr<class Camera> camera;
};
// ------------------------------- Callback Functions -------------------------------
extern Game* globalGame;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void enter_callback(GLFWwindow* window, int entered);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
