/*************************/
/*  FILE NAME: shader.h  */
/*************************/
#ifndef _SHADER_H_
#define _SHADER_H_

/**************/
/*  INCLUDES  */
/**************/
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <cstring>
#include <cstdlib>
#include <cstdio>

/************************/
/*  CLASS NAME: Shader  */
/************************/
class Shader
{
private:
    Shader() = delete;
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&);

public:
    Shader(const std::string& vert_file, const std::string& frag_file);
    ~Shader();

private:
    void init();
    void cleanup();

public:
    unsigned int get() const;

private:
    unsigned int program_id;
    std::string vert_file;
    std::string frag_file;
}; // class Shader

inline unsigned int Shader::get() const { return program_id; }
#endif // !_SHADER_H_