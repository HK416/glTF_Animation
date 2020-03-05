/***************************/
/*  FILE NAME: shader.cpp  */
/***************************/
#include "shader.h"

/**************/
/*  INCLUDES  */
/**************/
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/*************************/
/*  FUNCTION PROTOTYPES  */
/*************************/
void loadFromFile(const std::string&, char**);
void clearBuffer(char**);

std::string getCompileError(unsigned int);
std::string getLinkError(unsigned int);

// constructor 
Shader::Shader(const std::string& vert_file, const std::string& frag_file)
    : program_id(static_cast<unsigned int>(-1))
    , vert_file(vert_file)
    , frag_file(frag_file)
{
    init();
}

// destructor
Shader::~Shader()
{
    cleanup();
}

// function to create shader program
void Shader::init()
{
    unsigned int vertex_shader = static_cast<unsigned int>(-1);
    unsigned int fragment_shader = static_cast<unsigned int>(-1);
    char* buffer = nullptr;
    int status = GL_FALSE;

    // create vertex shader
    loadFromFile(vert_file, &buffer);
    {
        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &buffer, nullptr);

        glCompileShader(vertex_shader);
        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE)
            throw std::runtime_error(getCompileError(vertex_shader));
    }
    clearBuffer(&buffer);

    // create fragment shader
    loadFromFile(frag_file, &buffer);
    {
        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &buffer, nullptr);

        glCompileShader(fragment_shader);
        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE)
            throw std::runtime_error(getCompileError(fragment_shader));
    }
    clearBuffer(&buffer);

    // create shader program
    program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader);
    glAttachShader(program_id, fragment_shader);
    glLinkProgram(program_id);

    glGetProgramiv(program_id, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
        throw std::runtime_error(getLinkError(program_id));

    // clean up vertex shader and fragment shader
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

// function to clean up shader program
void Shader::cleanup()
{
    glDeleteProgram(program_id);
}

// function to read file
void loadFromFile(const std::string& filename, char** buffer)
{
    std::ifstream file(filename);
    if (file.is_open() == false)
        throw std::runtime_error("Failed to open \'" + filename + "\'");
    
    file.seekg(0, std::ios::end);
    size_t buffer_size = static_cast<size_t>(file.tellg());
    file.seekg(0, std::ios::beg);

    (*buffer) = new char[buffer_size + 1];
    memset((*buffer), 0, sizeof(char) * (buffer_size + 1));
    
    file.read(&(*buffer)[0], static_cast<std::streamsize>(buffer_size));
    file.close();
}

// function to clear buffer
void clearBuffer(char** buffer)
{
    if ((*buffer))
    {
        delete[] (*buffer);
        (*buffer) = nullptr;
    }
}

// funtion that return compile error message
std::string getCompileError(unsigned int shader)
{
    std::string msg = "";
    int length = 0;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    msg.resize(static_cast<size_t>(length));

    glGetShaderInfoLog(shader, length, &length, &msg[0]);
    return msg;
}

// function that return link error message
std::string getLinkError(unsigned int program)
{
    std::string msg = "";
    int length = 0;

    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    msg.resize(static_cast<size_t>(length));

    glGetProgramInfoLog(program, length, &length, &msg[0]);
    return msg;
}