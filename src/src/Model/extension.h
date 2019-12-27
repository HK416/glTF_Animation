/****************************/
/*  FILE NAME: extension.h  */
/****************************/
#ifndef _EXTENSION_H_
#define _EXTENSION_H_

/**************/
/*  INCLUDES  */
/**************/
#include <vector.hpp>

/***************************/
/*  CLASS NAME: Extension  */
/***************************/
class Extension
{
public:
    Extension();
    Extension(const Extension& other);

public:
    int specular_glossiness_texture_id;
    int diffuse_texture_id;
    orca::vec4<float> diffuse_factor;
    orca::vec3<float> specular_factor;
}; // class Extension
#endif // !_EXTENSION_H_