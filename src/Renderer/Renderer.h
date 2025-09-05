#pragma once

#include "../../include/glad/include/glad/glad.h"
#include <glm/glm.hpp>

class Renderer 
{
public:
    Renderer();
    ~Renderer();

    void renderSkybox(const glm::mat4& view, const glm::mat4& projection);

private:
    unsigned int skyboxVAO, skyboxVBO;
    GLuint cubemapTexture;
    GLuint skyboxShaderProgram;
    bool isSkyboxInitialized;
};
