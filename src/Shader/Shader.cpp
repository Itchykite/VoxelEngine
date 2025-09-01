#include "Shader.h"

Shader::Shader(const char* file, GLenum type) 
{
    std::string code;
    std::ifstream shaderFile;

    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
        shaderFile.open(file);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        code = shaderStream.str();
    } 
    catch (std::ifstream::failure& e) 
    {
        throw std::runtime_error("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
    }

    const char* shaderCode = code.c_str();
    ID = glCreateShader(type);
    glShaderSource(ID, 1, &shaderCode, nullptr);
    glCompileShader(ID);

    int success;
    char infoLog[512];
    glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
    if (!success) 
    {
        glGetShaderInfoLog(ID, 512, nullptr, infoLog);
        std::string errorMsg = "ERROR::SHADER::COMPILATION_FAILED\n";
        errorMsg += infoLog;
        throw std::runtime_error(errorMsg);
    }
}

Shader::~Shader() 
{
    glDeleteShader(ID);
}

GLuint CreateProgram(unsigned int vertexShader, unsigned int fragmentShader) 
{
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) 
    {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::string errorMsg = "ERROR::PROGRAM::LINKING_FAILED\n";
        errorMsg += infoLog;
        throw std::runtime_error(errorMsg);
    }

    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}
