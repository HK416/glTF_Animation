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

#include <memory.h>

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
    Shader(const std::string&, const std::string&);
    ~Shader();

private:
    void init();
    void exit();

    void loadFromFile(const std::string&, char**);
    void clearBuffer(char**);

    std::string getCompileError(unsigned int shader);
    std::string getLinkError(unsigned int program);

public:
    unsigned int get() const;

private:
    unsigned int program_id;
    std::string vert_file;
    std::string frag_file;
}; // class Shader

inline unsigned int Shader::get() const { return program_id; }
#endif // !_SHADER_H_