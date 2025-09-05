#include "World.h"
#include <iostream>

World::World() 
{
    try 
    {
        Shader vertexShader("../assets/shaders/chunk.vert", GL_VERTEX_SHADER);
        Shader fragmentShader("../assets/shaders/chunk.frag", GL_FRAGMENT_SHADER);
        m_shaderProgram = CreateProgram(vertexShader.ID, fragmentShader.ID);
    } 
    catch (const std::runtime_error& e) 
    {
        std::cerr << "Failed to create world shaders: " << e.what() << std::endl;
    }
    
    const int worldSize = 8;
    for (int x = -worldSize / 2; x < worldSize / 2; ++x) 
    {
        for (int z = -worldSize / 2; z < worldSize / 2; ++z) 
        {
            glm::ivec3 pos(x * Chunk::CHUNK_WIDTH, 0, z * Chunk::CHUNK_DEPTH);
            auto chunk = std::make_unique<Chunk>(pos);
            chunk->buildMesh();
            m_chunks[pos] = std::move(chunk);
        }
    }
}

World::~World() 
{
    glDeleteProgram(m_shaderProgram);
}

void World::render(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& viewPos) 
{
    glUseProgram(m_shaderProgram);

    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3fv(glGetUniformLocation(m_shaderProgram, "viewPos"), 1, glm::value_ptr(viewPos));
    
    glm::vec3 sunDir = glm::normalize(glm::vec3(-1.0f, -1.0f, -0.3f));
    glm::vec3 sunColor = glm::vec3(1.0f, 1.0f, 0.9f);
    glUniform3fv(glGetUniformLocation(m_shaderProgram, "sunDirection"), 1, &sunDir[0]);
    glUniform3fv(glGetUniformLocation(m_shaderProgram, "sunColor"), 1, &sunColor[0]);

    for (const auto& pair : m_chunks) 
    {
        glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(pair.second->getModelMatrix()));
        pair.second->render();
    }

    glBindVertexArray(0);
}
