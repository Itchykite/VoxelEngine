#include "Game.h"

#include <stdexcept>
#include <GL/gl.h>
#include <iostream>
#include "../../include/imgui/imgui.h"
#include "../../include/imgui/backends/imgui_impl_glfw.h"
#include "../../include/imgui/backends/imgui_impl_opengl3.h"
#include "../Shader/Shader.h"
#include "../Renderer/Renderer.h"
#include "../Camera/Camera.h"

// Global variables for mouse callback
double lastX = 400, lastY = 300;
Camera* globalCamera = nullptr;
Game* globalGame = nullptr;

// Debug variables - dodane dla lepszej diagnostyki
double deltaX = 0.0, deltaY = 0.0;
bool mouseMoved = false;

// ------------------------------- Main Functions -------------------------------
Game::Game() : cameraControlEnabled(false) {}

Game::~Game() 
{
    cleanup();
}

void Game::run() 
{
    init();
    loop();
}

void Game::init()
{
    if (!glfwInit()) 
    {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
    window = glfwCreateWindow(windowWidth, windowHeight, "Game Window", nullptr, nullptr);
    if (!window) 
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
    {
        glfwDestroyWindow(window);
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD");
    }

    glViewport(0, 0, windowWidth, windowHeight);
    glEnable(GL_DEPTH_TEST);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    
    ImGui_ImplGlfw_InitForOpenGL(window, false);
    ImGui_ImplOpenGL3_Init("#version 330");

    camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
    globalCamera = camera.get();
    globalGame = this;

    if (windowHeight > 0) 
    {
        float initialAspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
        camera->setAspect(initialAspectRatio);
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, keyboard_callback);
    
    renderer = std::make_unique<Renderer>();
    
    glfwGetCursorPos(window, &lastX, &lastY);
}

void Game::loop() 
{
    double currentTime, lastFrame = glfwGetTime();
    double deltaTime;
    
    double currentX, currentY;
    
    while (!glfwWindowShouldClose(window)) 
    {
        currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;
        
        glfwPollEvents();

        glfwGetCursorPos(window, &currentX, &currentY);

        if (cameraControlEnabled && globalCamera)
        {
            deltaX = currentX - lastX;
            deltaY = lastY - currentY; 
            
            mouseMoved = (deltaX != 0.0 || deltaY != 0.0);
            
            if (mouseMoved)
            {
                float sensitivity = 0.1f;
                float xoffset = static_cast<float>(deltaX) * sensitivity;
                float yoffset = static_cast<float>(deltaY) * sensitivity;
                
                globalCamera->processMouseMovement(xoffset, yoffset);
            }
        }
        
        lastX = currentX;
        lastY = currentY;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("FPS");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
        
        // UI ImGui
        ImGui::Begin("Camera Controls");
        ImGui::Text("Press LMB to control camera");
        ImGui::Text("Mouse Position: x: %.1f, y: %.1f", static_cast<float>(currentX), static_cast<float>(currentY));
        ImGui::Text("Mouse Delta: x: %.1f, y: %.1f", static_cast<float>(deltaX), static_cast<float>(deltaY));
        ImGui::Text("Camera Control: %s", cameraControlEnabled ? "ON" : "OFF");
        ImGui::Text("Mouse Moved: %s", mouseMoved ? "YES" : "NO");
        
        if (globalCamera) 
        {
            ImGui::Text("Camera Yaw: %.1f, Pitch: %.1f", globalCamera->yaw, globalCamera->pitch);
            ImGui::Text("Camera Position: %.1f, %.1f, %.1f", 
                     globalCamera->Position.x, globalCamera->Position.y, globalCamera->Position.z);
            
            if (ImGui::Button("Look Left")) 
                globalCamera->processMouseMovement(-10.0f, 0.0f);
            ImGui::SameLine();
            if (ImGui::Button("Look Right"))
                globalCamera->processMouseMovement(10.0f, 0.0f);
            ImGui::NewLine();
            if (ImGui::Button("Look Up"))
                globalCamera->processMouseMovement(0.0f, 5.0f);
            ImGui::SameLine();
            if (ImGui::Button("Look Down")) 
                globalCamera->processMouseMovement(0.0f, -5.0f);
        }
        
        ImGui::End();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        glm::mat4 view = camera->getViewMatrix(); 
        glm::mat4 projection = camera->getProjectionMatrix();
        renderer->renderSkybox(view, projection);

        renderer->renderCube(view, projection, glm::vec3(0.0f, 0.0f, 0.0f), camera->Position, glm::vec3(1.0f, 0.5f, 0.31f));
        renderer->renderCube(view, projection, glm::vec3(2.0f, 0.0f, -1.0f), camera->Position, glm::vec3(0.0f, 0.5f, 0.31f));
        renderer->renderCube(view, projection, glm::vec3(-1.5f, 0.0f, -2.0f), camera->Position, glm::vec3(0.0f, 0.5f, 0.31f));
        renderer->renderCube(view, projection, glm::vec3(-3.0f, 1.0f, -3.0f), camera->Position, glm::vec3(0.0f, 0.5f, 0.31f));
        renderer->renderCube(view, projection, glm::vec3(1.5f, -1.0f, -2.5f), camera->Position, glm::vec3(0.0f, 0.5f, 0.31f));

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);

        processKeyboardInput(deltaTime);
    }
}

void Game::processKeyboardInput(float deltaTime)
{
    float cameraSpeed = 1.0f * deltaTime;
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        globalCamera->processKeyboardMovement(0.0f, 0.0f, cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        globalCamera->processKeyboardMovement(0.0f, 0.0f, -cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        globalCamera->processKeyboardMovement(-cameraSpeed, 0.0f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        globalCamera->processKeyboardMovement(cameraSpeed, 0.0f, 0.0f);
   
    // Arrow keys for looking around
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        globalCamera->processMouseMovement(0.0f, 1.0f);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        globalCamera->processMouseMovement(0.0f, -1.0f);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        globalCamera->processMouseMovement(-1.0f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        globalCamera->processMouseMovement(1.0f, 0.0f);
}

void Game::cleanup() 
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    if (window) 
    {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}

// ------------------------------- Callbacks -------------------------------
void framebuffer_size_callback(GLFWwindow* /* window */, int width, int height) 
{
    glViewport(0, 0, width, height);

    if (globalCamera && height > 0) 
    { 
        float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
        globalCamera->setAspect(aspectRatio);
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
    
    ImGuiIO& io = ImGui::GetIO();
    if (!io.WantCaptureMouse)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            if (action == GLFW_PRESS)
            {
                if (globalGame) 
                {
                    globalGame->cameraControlEnabled = true;
                    
                    glfwGetCursorPos(window, &lastX, &lastY);
                }
            }
            else if (action == GLFW_RELEASE)
            {
                if (globalGame) 
                {
                    globalGame->cameraControlEnabled = false;
                }
            }
        }
    }
}

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
    
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
