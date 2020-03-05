/*************************/
/*  FILE NAME: camera.h  */
/*************************/
#ifndef _CAMERA_H_
#define _CAMERA_H_

/**************/
/*  INCLUDES  */
/**************/
#include <vector.hpp>
#include <vector_functions.hpp>
#include <matrix.hpp>
#include <matrix_functions.hpp>
#include <quaternion.hpp>
#include <quaternion_functions.hpp>

/*************************/
/*  STRUCT NAME: Camera  */
/*************************/
struct Camera
{
    orca::mat4f getProjectionMatrix(int width, int height) const;
    orca::mat4f getViewMatrix() const;

    orca::vec3f pos = orca::vec3f(0, 0, 0);
    orca::vec3f dir = orca::vec3f(0, 0, 1);
    orca::vec3f up = orca::vec3f(0, 1, 0);

    double yaw = 0.0;
    double pitch = 0.0;
    double roll = 0.0;
    double fovy = 45.0;
    double speed = 1.0;
}; // struct Camera
#endif // !_CAMERA_H_