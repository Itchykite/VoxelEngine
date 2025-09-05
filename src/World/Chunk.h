#pragma once

#include "Block.h"
#include <vector>
#include <array>
#include <glm/glm.hpp>
#include "../../include/glad/include/glad/glad.h"

class Chunk {
public:
    static constexpr int CHUNK_WIDTH = 16;
    static constexpr int CHUNK_HEIGHT = 16;
    static constexpr int CHUNK_DEPTH = 16;

    Chunk(const glm::ivec3& position);
    ~Chunk();

    void buildMesh();
    void render();

    BlockType getBlock(int x, int y, int z) const;
    void setBlock(int x, int y, int z, BlockType type);
    
    glm::mat4 getModelMatrix() const;

private:
    std::array<BlockType, CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH> m_blocks;
    glm::ivec3 m_position; 

    GLuint m_vao = 0, m_vbo = 0;
    int m_vertexCount = 0;

    bool isPositionInChunk(int x, int y, int z) const;

    // NOWA DEKLARACJA: Prywatna metoda pomocnicza do pobierania kolorów
    static glm::vec3 getBlockColor(BlockType type); 
};
