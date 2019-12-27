/*************************/
/*  FILE NAME: node.cpp  */
/*************************/
#include "node.h"

/**************/
/*  INCLUDES  */
/**************/
#include <vector_functions.hpp>
#include <matrix_functions.hpp>
#include <quaternion_functions.hpp>

/* default constructor */
Node::Node()
    : name()
    , child_ids()
    , matrix()
    , translate()
    , rotate()
    , scale()
    , node_id()
    , parent_id()
    , mesh_id()
    , skin_id()
{
    matrix = orca::Identity(matrix);
    scale = orca::vec3<float>(1.0f);
    node_id = -1;
    parent_id = -1;
    mesh_id = -1;
    skin_id = -1;
}

/* copy constructor */
Node::Node(const Node& other)
    : name(other.name)
    , child_ids(other.child_ids)
    , matrix(other.matrix)
    , translate(other.translate)
    , rotate(other.rotate)
    , scale(other.scale)
    , node_id(other.node_id)
    , parent_id(other.parent_id)
    , mesh_id(other.mesh_id)
    , skin_id(other.skin_id)
{ /* empty */ }

/* function to return local translation of node */
orca::mat4<float> Node::LocalMatrix() const
{
    return matrix * orca::Scale(scale) * orca::QuaternionToMatrix(orca::quaternion(rotate)) * orca::Translate(translate);
}