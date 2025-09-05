#pragma once

#include "Chunk.h"
#include "../Shader/Shader.h"
#include <map>
#include <memory>
#include <glm/gtc/matrix_transform.hpp>

struct ivec3_comp 
{
    bool operator()(const glm::ivec3& a, const glm::ivec3& b) const 
    {
        if (a.x != b.x) return a.x < b.x;
        if (a.y != b.y) return a.y < b.y;
        return a.z < b.z;
    }
};

class World 
{
public:
    World();
    ~World();

    void render(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& viewPos);

private:
    std::map<glm::ivec3, std::unique_ptr<Chunk>, ivec3_comp> m_chunks;
    GLuint m_shaderProgram;
};
