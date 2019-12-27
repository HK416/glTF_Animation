/*************************/
/*  FILE NAME: mesh.cpp  */
/*************************/
#include "mesh.h"

/**************/
/*  INCLUDES  */
/**************/
#include <GL/glew.h>
#include <vector_functions.hpp>
#include <matrix_functions.hpp>

/* default constructor */
Mesh::Mesh()
    : name()
    , vertices()
    , indices()
    , material_id()
    , matrix()
    , joint_matrices()
    , vao()
    , vbo()
    , ebo()
{ 
    for(unsigned int i = 0; i < MAX_NUM_JOINTS; ++i)
        joint_matrices[i] = orca::Identity(joint_matrices[i]);

    material_id = -1;
    vao = 0;
    vbo = 0;
    ebo = 0;
}

/* copy constructor */
Mesh::Mesh(const Mesh& other)
    : name(other.name)
    , vertices(other.vertices)
    , indices(other.indices)
    , material_id(other.material_id)
    , matrix(other.matrix)
    , joint_matrices(other.joint_matrices)
    , vao(other.vao)
    , vbo(other.vbo)
    , ebo(other.ebo)
{ /* empty */ }

/* set the mesh data to be available in OpenGL */
void Mesh::SetupMesh()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texcoord)));

    glEnableVertexAttribArray(3);
    glVertexAttribIPointer(3, 4, GL_UNSIGNED_INT, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, joint)));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, weight)));

    glGenVertexArrays(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

/* clean up the mesh data that was set up */
void Mesh::CleanupMesh()
{
    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

/* render the mesh data */
void Mesh::Render(unsigned int shader_program, bool is_animation)
{
    if(is_animation == true)
        glUniformMatrix4fv(glGetUniformLocation(shader_program, "bone_matrix"), MAX_NUM_JOINTS, GL_FALSE, joint_matrices[0]);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, indices.size());
    glBindVertexArray(0);
}
