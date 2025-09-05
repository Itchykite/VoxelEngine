#include "Chunk.h"
#include <glm/gtc/matrix_transform.hpp>

glm::vec3 getBlockColor(BlockType type) 
{
    switch (type) 
    {
        case BlockType::Grass: return glm::vec3(0.0f, 0.8f, 0.2f);
        case BlockType::Dirt:  return glm::vec3(0.5f, 0.35f, 0.2f);
        case BlockType::Stone: return glm::vec3(0.5f, 0.5f, 0.5f);
        default:               return glm::vec3(1.0f, 0.0f, 1.0f); 
    }
}

Chunk::Chunk(const glm::ivec3& position) : m_position(position) 
{
    for (int x = 0; x < CHUNK_WIDTH; ++x) 
        for (int z = 0; z < CHUNK_DEPTH; ++z) 
            for (int y = 0; y < CHUNK_HEIGHT; ++y) 
                if (y < CHUNK_HEIGHT / 2 - 2) 
                    setBlock(x, y, z, BlockType::Stone);
                else if (y < CHUNK_HEIGHT / 2) 
                    setBlock(x, y, z, BlockType::Dirt);
                else if (y == CHUNK_HEIGHT / 2) 
                    setBlock(x, y, z, BlockType::Grass);
                else 
                    setBlock(x, y, z, BlockType::Air);
}

Chunk::~Chunk() 
{
    if (m_vao != 0) glDeleteVertexArrays(1, &m_vao);
    if (m_vbo != 0) glDeleteBuffers(1, &m_vbo);
}

void Chunk::setBlock(int x, int y, int z, BlockType type) 
{
    if (isPositionInChunk(x, y, z)) 
    {
        m_blocks[y * CHUNK_WIDTH * CHUNK_DEPTH + z * CHUNK_WIDTH + x] = type;
    }
}

BlockType Chunk::getBlock(int x, int y, int z) const 
{
    if (!isPositionInChunk(x, y, z)) 
    {
        return BlockType::Air;
    }
    return m_blocks[y * CHUNK_WIDTH * CHUNK_DEPTH + z * CHUNK_WIDTH + x];
}

bool Chunk::isPositionInChunk(int x, int y, int z) const {
    return x >= 0 && x < CHUNK_WIDTH && y >= 0 && y < CHUNK_HEIGHT && z >= 0 && z < CHUNK_DEPTH;
}

void Chunk::buildMesh() {
    std::vector<float> vertices;
    const float size = 1.0f;

    for (int y = 0; y < CHUNK_HEIGHT; ++y) 
    {
        for (int x = 0; x < CHUNK_WIDTH; ++x) 
        {
            for (int z = 0; z < CHUNK_DEPTH; ++z) 
            {
                BlockType currentBlockType = getBlock(x, y, z);
                if (currentBlockType == BlockType::Air) continue;

                // Top (+Y)
                if (y == CHUNK_HEIGHT - 1 || getBlock(x, y + 1, z) == BlockType::Air) 
                {
                    glm::vec3 color = getBlockColor(BlockType::Grass); 
                    vertices.insert(vertices.end(), 
                    {
                        x, y + size, z,         0, 1, 0,  color.r, color.g, color.b,
                        x, y + size, z + size,   0, 1, 0,  color.r, color.g, color.b,
                        x + size, y + size, z,      0, 1, 0,  color.r, color.g, color.b,
                        x + size, y + size, z,      0, 1, 0,  color.r, color.g, color.b,
                        x, y + size, z + size,   0, 1, 0,  color.r, color.g, color.b,
                        x + size, y + size, z + size, 0, 1, 0,  color.r, color.g, color.b
                    });
                }
                // Bottom (-Y)
                if (y == 0 || getBlock(x, y - 1, z) == BlockType::Air) 
                {
                    glm::vec3 color = getBlockColor(BlockType::Dirt); 
                    vertices.insert(vertices.end(), 
                    {
                        x, y, z,         0, -1, 0,  color.r, color.g, color.b,
                        x + size, y, z,      0, -1, 0,  color.r, color.g, color.b,
                        x, y, z + size,   0, -1, 0,  color.r, color.g, color.b,
                        x + size, y, z,      0, -1, 0,  color.r, color.g, color.b,
                        x + size, y, z + size, 0, -1, 0,  color.r, color.g, color.b,
                        x, y, z + size,   0, -1, 0,  color.r, color.g, color.b
                    });
                }

                // (Front, Back, Left, Right)
                glm::vec3 sideColor = getBlockColor(currentBlockType == BlockType::Grass ? BlockType::Dirt : currentBlockType);
                // Front (+Z)
                if (z == CHUNK_DEPTH - 1 || getBlock(x, y, z + 1) == BlockType::Air) 
                {
                    vertices.insert(vertices.end(), 
                    {
                        x, y, z + size,       0, 0, 1,  sideColor.r, sideColor.g, sideColor.b,
                        x + size, y, z + size, 0, 0, 1,  sideColor.r, sideColor.g, sideColor.b,
                        x, y + size, z + size, 0, 0, 1,  sideColor.r, sideColor.g, sideColor.b,
                        x + size, y, z + size, 0, 0, 1,  sideColor.r, sideColor.g, sideColor.b,
                        x + size, y + size, z + size, 0, 0, 1,  sideColor.r, sideColor.g, sideColor.b,
                        x, y + size, z + size, 0, 0, 1,  sideColor.r, sideColor.g, sideColor.b
                    });
                }
                // Back (-Z)
                if (z == 0 || getBlock(x, y, z - 1) == BlockType::Air) 
                {
                    vertices.insert(vertices.end(), 
                    {
                        x, y, z,       0, 0, -1,  sideColor.r, sideColor.g, sideColor.b,
                        x, y + size, z, 0, 0, -1,  sideColor.r, sideColor.g, sideColor.b,
                        x + size, y, z, 0, 0, -1,  sideColor.r, sideColor.g, sideColor.b,
                        x + size, y, z, 0, 0, -1,  sideColor.r, sideColor.g, sideColor.b,
                        x, y + size, z, 0, 0, -1,  sideColor.r, sideColor.g, sideColor.b,
                        x + size, y + size, z, 0, 0, -1,  sideColor.r, sideColor.g, sideColor.b
                    });
                }
                 // Right (+X)
                if (x == CHUNK_WIDTH - 1 || getBlock(x + 1, y, z) == BlockType::Air) 
                {
                     vertices.insert(vertices.end(), 
                     {
                        x + size, y, z, 1, 0, 0,  sideColor.r, sideColor.g, sideColor.b,
                        x + size, y + size, z, 1, 0, 0,  sideColor.r, sideColor.g, sideColor.b,
                        x + size, y, z + size, 1, 0, 0,  sideColor.r, sideColor.g, sideColor.b,
                        x + size, y + size, z, 1, 0, 0,  sideColor.r, sideColor.g, sideColor.b,
                        x + size, y + size, z + size, 1, 0, 0,  sideColor.r, sideColor.g, sideColor.b,
                        x + size, y, z + size, 1, 0, 0,  sideColor.r, sideColor.g, sideColor.b
                    });
                }
                // Left (-X)
                if (x == 0 || getBlock(x - 1, y, z) == BlockType::Air) 
                {
                    vertices.insert(vertices.end(), 
                    {
                        x, y, z, -1, 0, 0,  sideColor.r, sideColor.g, sideColor.b,
                        x, y, z + size, -1, 0, 0,  sideColor.r, sideColor.g, sideColor.b,
                        x, y + size, z, -1, 0, 0,  sideColor.r, sideColor.g, sideColor.b,
                        x, y + size, z, -1, 0, 0,  sideColor.r, sideColor.g, sideColor.b,
                        x, y, z + size, -1, 0, 0,  sideColor.r, sideColor.g, sideColor.b,
                        x, y + size, z + size, -1, 0, 0,  sideColor.r, sideColor.g, sideColor.b
                    });
                }
            }
        }
    }
    
    m_vertexCount = vertices.size() / 9;  

    if (m_vao != 0) glDeleteVertexArrays(1, &m_vao);
    if (m_vbo != 0) glDeleteBuffers(1, &m_vbo);

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    const int stride = 9 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Chunk::render()
{
    if (m_vertexCount > 0) 
    {
        glBindVertexArray(m_vao);
        glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
        glBindVertexArray(0);
    }
}

glm::mat4 Chunk::getModelMatrix() const 
{
    return glm::translate(glm::mat4(1.0f), glm::vec3(m_position));
}
