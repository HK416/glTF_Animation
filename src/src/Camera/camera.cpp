/***************************/
/*  FILE NAME: camera.cpp  */
/***************************/
#include "camera.h"

// this function returns the projection matrix
orca::mat4f Camera::getProjectionMatrix(int width, int height) const
{
    float aspect = static_cast<float>(width) / static_cast<float>(height);
    return orca::PerspectiveRH(orca::DegreeToRadian<float>(fovy), aspect, 0.1f, 1000.0f);
}

// this function returns the view matrix
orca::mat4f Camera::getViewMatrix() const
{
    return orca::LookAtRH(pos, pos + dir, up);
}