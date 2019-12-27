/***********************/
/*  FILE NAME: mesh.h  */
/***********************/
#ifndef _MESH_H_
#define _MESH_H_

/**************/
/*  INCLUDES  */
/**************/
#include <string>
#include <vector>
#include <array>
#include <vector.hpp>
#include <matrix.hpp>
#include "vertex.h"

constexpr unsigned int MAX_NUM_JOINTS = 128U;

/**********************/
/*  CLASS NAME: Mesh  */
/**********************/
class Mesh
{
public:
    Mesh();
    Mesh(const Mesh& other);

public:
    void SetupMesh();
    void CleanupMesh();
    void Render(unsigned int shader_program, bool is_animation);

public:
    std::string name;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    int material_id;
    orca::mat4<float> matrix;
    std::array<orca::mat4<float>, MAX_NUM_JOINTS> joint_matrices;

private:
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
}; // class Mesh
#endif // !_MESH_H_