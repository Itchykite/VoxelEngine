#include "Renderer.h"
#include "../Shader/Shader.h"
#include "../TextureLoader/TextureLoader.h"
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

std::vector<float> skyboxVertices = 
{
    -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f
};

Renderer::Renderer() : isSkyboxInitialized(false), skyboxVAO(0), skyboxVBO(0), cubemapTexture(0), skyboxShaderProgram(0) {}

Renderer::~Renderer() 
{
    if (skyboxVAO != 0) glDeleteVertexArrays(1, &skyboxVAO);
    if (skyboxVBO != 0) glDeleteBuffers(1, &skyboxVBO);
    if (skyboxShaderProgram != 0) glDeleteProgram(skyboxShaderProgram);
    if (cubemapTexture != 0) glDeleteTextures(1, &cubemapTexture);
}

void Renderer::renderSkybox(const glm::mat4& view, const glm::mat4& projection) 
{
    if (!isSkyboxInitialized) 
    {
        glGenVertexArrays(1, &skyboxVAO);
        glGenBuffers(1, &skyboxVBO);
        glBindVertexArray(skyboxVAO);
        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER, skyboxVertices.size() * sizeof(float), &skyboxVertices[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        std::vector<std::string> faces = 
        {
            "../assets/textures/skybox/right.bmp", "../assets/textures/skybox/left.bmp",
            "../assets/textures/skybox/top.bmp",   "../assets/textures/skybox/bottom.bmp",
            "../assets/textures/skybox/front.bmp", "../assets/textures/skybox/back.bmp"
        };
        cubemapTexture = loadCubemap(faces);

        Shader vertexShader("../assets/shaders/skybox.vert", GL_VERTEX_SHADER);
        Shader fragmentShader("../assets/shaders/skybox.frag", GL_FRAGMENT_SHADER);
        skyboxShaderProgram = CreateProgram(vertexShader.ID, fragmentShader.ID);

        isSkyboxInitialized = true;
    }

    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);
    glUseProgram(skyboxShaderProgram);

    glm::mat4 onlyRot = glm::mat4(glm::mat3(view));
    glUniformMatrix4fv(glGetUniformLocation(skyboxShaderProgram, "view"), 1, GL_FALSE, &onlyRot[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(skyboxShaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glUniform1i(glGetUniformLocation(skyboxShaderProgram, "skybox"), 0);

    glBindVertexArray(skyboxVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}
