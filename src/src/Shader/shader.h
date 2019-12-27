/*************************/
/*  FILE NAME: shader.h  */
/*************************/
#ifndef _SHADER_H_
#define _SHADER_H_

unsigned int CreateShaderProgram(const char* vert_file, const char* frag_file);
void DeleteShaderProgram(unsigned int shader_program);

#endif // !_SHADER_H_