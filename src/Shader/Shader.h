#pragma once

#include "../../include/glad/include/glad/glad.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader 
{
public:
    unsigned int ID;

    Shader(const char* file, GLenum type);
    ~Shader();
};

GLuint CreateProgram(unsigned int vertexShader, unsigned int fragmentShader); 
