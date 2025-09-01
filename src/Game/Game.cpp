#include "Game.h"

#include <stdexcept>
#include <GL/gl.h>
#include "../../include/imgui/imgui.h"
#include "../../include/imgui/backends/imgui_impl_glfw.h"
#include "../../include/imgui/backends/imgui_impl_opengl3.h"
#include "../Shader/Shader.h"
#include "../Renderer/Renderer.h"
#include "../Camera/Camera.h"

// Global variables for mouse callback 
float lastX = 400, lastY = 300;
bool firstMouse = true;
std::unique_ptr<Camera> globalCamera;

// ------------------------------- Main Functions -------------------------------
Game::Game() {}
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
        
    window = glfwCreateWindow(800, 600, "Game Window", nullptr, nullptr);
    if (!window) 
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
    {
        glfwDestroyWindow(window);
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD");
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glEnable(GL_DEPTH_TEST);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
    globalCamera = std::move(camera);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, keyboard_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    renderer = std::make_unique<Renderer>();
    camera = std::move(globalCamera);
}

void Game::loop() 
{
    while (!glfwWindowShouldClose(window)) 
    {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Hello, ImGui!");
        ImGui::Text("This is a simple ImGui window.");
        ImGui::End();

        glm::mat4 view = camera->getViewMatrix(); 
        glm::mat4 projection = camera->getProjectionMatrix();
        renderer->renderSkybox(view, projection);

        renderer->renderCube();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
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
void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
  
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    
    lastX = xpos;
    lastY = ypos;

    if (globalCamera)
        globalCamera->processMouseMovement(xoffset, yoffset);
}

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (globalCamera && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        if (key == GLFW_KEY_W)
            globalCamera->processKeyboardMovement(0.0f, 0.0f, 0.1f);
        if (key == GLFW_KEY_S)
            globalCamera->processKeyboardMovement(0.0f, 0.0f, -0.1f);
        if (key == GLFW_KEY_A)
            globalCamera->processKeyboardMovement(-0.1f, 0.0f, 0.0f);
        if (key == GLFW_KEY_D)
            globalCamera->processKeyboardMovement(0.1f, 0.0f, 0.0f);
    }
}
