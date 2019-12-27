/***********************/
/*  FILE NAME: node.h  */
/***********************/
#ifndef _NODE_H_
#define _NODE_H_

/**************/
/*  INCLUDES  */
/**************/
#include <string>
#include <vector>
#include <vector.hpp>
#include <matrix.hpp>

/**********************/
/*  CLASS NAME: Node  */
/**********************/
class Node
{
public:
    Node();
    Node(const Node& other);

public:
    orca::mat4<float> LocalMatrix() const;

public:
    std::string name;
    std::vector<int> child_ids;

    orca::mat4<float> matrix;
    orca::vec3<float> translate;
    orca::vec4<float> rotate;
    orca::vec3<float> scale;

    int node_id;
    int parent_id;
    int mesh_id;
    int skin_id;
}; // class Node
#endif // !_NODE_H_