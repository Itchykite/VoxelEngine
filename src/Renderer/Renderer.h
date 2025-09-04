#pragma once

#include "../../include/glad/include/glad/glad.h"
#include <glm/glm.hpp>

class Renderer 
{
public:
    Renderer();
    ~Renderer();

    void renderSkybox(const glm::mat4& view, 
                      const glm::mat4& projection);

    void renderCube(const glm::mat4& view,
                    const glm::mat4& projection, 
                    const glm::vec3& position,
                    const glm::vec3& viewPos, 
                    const glm::vec3& objectColor, 
                    const glm::vec3& scale = glm::vec3(1.0f),
                    GLuint Texture = 0);

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
