/************************/
/* FILE NAME: vertex.h  */
/************************/
#ifndef _VERTEX_H_
#define _VERTEX_H_

/**************/
/*  INCLUDES  */
/**************/
#include <vector.hpp>

/************************/
/*  CLASS NAME: Vertex  */
/************************/
class Vertex
{
public:
    Vertex();
    Vertex(const Vertex& other);

public:
    orca::vec3<float> position;
    orca::vec3<float> normal;
    orca::vec2<float> texcoord;
    orca::vec4<unsigned int> joint;
    orca::vec4<float> weight;
}; // class Vertex
#endif // !_VERTEX_H_