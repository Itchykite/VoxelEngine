#pragma once

#include "../../include/glad/include/glad/glad.h"
#include <glm/glm.hpp>

class Renderer 
{
public:
    Renderer();
    ~Renderer();

    void renderSkybox(const glm::mat4& view, const glm::mat4& projection);
    void renderCube(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& viewPos);

private:
    // Skybox 
    unsigned int skyboxVAO, skyboxVBO;
    GLuint cubemapTexture;
    bool isSkyboxInitialized;

    // Cube
    unsigned int cubeVAO, cubeVBO;
    GLuint cubeShaderProgram;
    bool isCubeInitialized;

    GLuint shaderProgram;
};
