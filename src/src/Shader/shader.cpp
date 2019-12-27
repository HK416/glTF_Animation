/***************************/
/*  FILE NAME: shader.cpp  */
/***************************/
#include "shader.h"

/**************/
/*  INCLUDES  */
/**************/
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <GL/glew.h>

/* load the shader file and store it in the buffer */
bool LoadShaderSource(const char* file, char*& buffer)
{
    std::ifstream shader_file(file);
    if(shader_file.is_open() == false)
    {
        std::cout << "Failed to open \'" << std::string(file) << "\'" << std::endl;
        return false;
    }

    shader_file.seekg(0, std::ios::end);
    unsigned int buffer_size = static_cast<unsigned int>(shader_file.tellg());
    shader_file.seekg(0, std::ios::beg);

    buffer = new char[buffer_size + 1];
    for(unsigned int i = 0; i < buffer_size + 1; ++i)
        buffer[i] = 0;
    
    shader_file.read(&buffer[0], static_cast<std::streamsize>(buffer_size));
    shader_file.close();

    std::cout << std::endl;
    std::cout << "File: " << std::string(file) << std::endl;
    std::cout << std::string(buffer) << std::endl;
    std::cout << std::endl;
    return true;
}

/* delete the buffer */
void DeleteShaderSource(char*& buffer)
{
    if(buffer != nullptr)
    {
        delete[] buffer;
        buffer = nullptr;
    }
}

/* print shader compilation errors to the screen. */
void PrintCompileErrorMessage(unsigned int shader)
{
    char* message = nullptr;
    int length = 0;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    message = new char[length + 1];
    
    for(int i = 0; i < length + 1; ++i)
        message[i] = 0;

    glGetShaderInfoLog(shader, length, &length, &message[0]);
    std::cout << "<COMPILE ERROR>" << std::string(message) << std::endl;

    delete[] message;
    message = nullptr;
}

/* print shader link errors to the screen. */
void PrintLinkErrorMessage(unsigned int program)
{
    char* message = nullptr;
    int length = 0;

    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    message = new char[length + 1];

    for(int i = 0; i < length + 1; ++i)
        message[i] = 0;

    glGetProgramInfoLog(program, length, &length, &message[0]);
    std::cout << "<LINK ERROR>" << std::string(message) << std::endl;

    delete[] message;
    message = nullptr;
}

/* create a shader program */
unsigned int CreateShaderProgram(const char* vert_file, const char* frag_file)
{
    unsigned int program = 0;
    unsigned int vertex_shader = 0;
    unsigned int fragment_shader = 0;
    int status = GL_FALSE;
    char* buffer = nullptr;

    /* create vertex shader */
    LoadShaderSource(vert_file, buffer);
    {
        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &buffer, nullptr);

        glCompileShader(vertex_shader);
        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);
        if(status == GL_FALSE)
        {
            PrintCompileErrorMessage(vertex_shader);
            return 0;
        }
    }
    DeleteShaderSource(buffer);

    /* create fragment shader */
    LoadShaderSource(frag_file, buffer);
    {
        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &buffer, nullptr);

        glCompileShader(fragment_shader);
        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &status);
        if(status == GL_FALSE)
        {
            PrintCompileErrorMessage(fragment_shader);
            return 0;
        }
    }
    DeleteShaderSource(buffer);

    /* create shader program */
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if(status == GL_FALSE)
    {
        PrintLinkErrorMessage(program);
        return 0;
    }

    return program;
}

/* remove the shader program */
void DeleteShaderProgram(unsigned int shader_program)
{
    glDeleteProgram(shader_program);
}