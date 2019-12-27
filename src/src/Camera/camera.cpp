/***************************/
/*  FILE NAME: camera.cpp  */
/***************************/
#include "camera.h"

/**************/
/*  INCLUDES  */
/**************/
#include <cmath>
#include <algorithm>
#include <vector_functions.hpp>
#include <matrix_functions.hpp>
#include <quaternion_functions.hpp>

/* default constructor */
Camera::Camera()
    : position()
    , direction()
    , up()
    , yaw()
    , pitch()
    , roll()
    , screen_width()
    , screen_height()
    , fovy()
    , camera_speed()
{ 
    position = orca::vec3<float>(0, 0, 0);
    direction = orca::vec3<float>(0, 0, 1);
    up = orca::vec3<float>(0, 1, 0);
    yaw = 0.0f;
    pitch = 0.0f;
    roll = 0.0f;
    screen_width = 1280;
    screen_height = 720;
    fovy = 45.0f;
    camera_speed = 1.0f;
}

/* copy constructor */
Camera::Camera(const Camera& other)
    : position(other.position)
    , direction(other.direction)
    , up(other.up)
    , yaw(other.yaw)
    , pitch(other.pitch)
    , roll(other.roll)
    , screen_width(other.screen_width)
    , screen_height(other.screen_height)
    , fovy(other.fovy)
    , camera_speed(other.camera_speed)
{ /* empty */ }

/* return screen width */
int Camera::GetScreenWidth() const
{
    return screen_width;
}

/* return screen height */
int Camera::GetScreenHeight() const
{
    return screen_height;
}

/* return view (look at) matrix */
orca::mat4<float> Camera::GetViewMatrix() const
{
    return orca::LookAtRH(position, position + direction, up);
}

/* return projection matrix */
orca::mat4<float> Camera::GetProjectionMatrix() const
{
    float aspect = static_cast<float>(screen_width) / static_cast<float>(screen_height);
    return orca::PerspectiveRH(orca::DegreeToRadian(fovy), aspect, 0.1f, 1000.0f);
}

/* change the screen size when resizing the window */
void Camera::ScreenResize(int width, int height)
{
    screen_width = width;
    screen_height = height;
}

/* change the filed of vision y as srcoll the mouse */
void Camera::MouseScroll(float x_offset, float y_offset)
{
    fovy = std::clamp(fovy - y_offset, 1.0f, 45.0f);
}

/* change the direction vector as the mouse moves */
void Camera::MouseMovement(float x_offset, float y_offset)
{
    x_offset *= mouse_sensitivity;
    y_offset *= mouse_sensitivity;

    yaw += x_offset;
    pitch = std::clamp(pitch + y_offset, -89.0f, 89.0f);
    
    direction.x = std::cos(orca::DegreeToRadian(yaw)) * std::cos(orca::DegreeToRadian(pitch));
    direction.y = std::sin(orca::DegreeToRadian(pitch));
    direction.z = std::sin(orca::DegreeToRadian(yaw)) * std::cos(orca::DegreeToRadian(pitch));
}

/* change the position vector as the keyboard input */
void Camera::CameraTranslation(CameraMovement movement, float delta_time)
{
    float velocity = camera_speed * delta_time;
    orca::vec3<float> front = orca::Normalize(10.0f * orca::vec3<float>(direction.x, 0, direction.z));
    orca::vec3<float> left = orca::Normalize(10.0f * orca::Cross(up, front));
    orca::vec3<float> up = orca::Normalize(this->up);

    position += front * velocity * (movement == CameraMovement::FORWARD);
    position -= front * velocity * (movement == CameraMovement::BACKWARD);
    position += left * velocity * (movement == CameraMovement::RIGHT);
    position -= left * velocity * (movement == CameraMovement::LEFT);
    position += up * velocity * (movement == CameraMovement::UP);
    position -= up * velocity * (movement == CameraMovement::DOWN);
}

/* function to change the speed of the camera */
void Camera::CameraSpeed(Speed speed)
{
    camera_speed = std::clamp(camera_speed + static_cast<float>(speed) * 0.1f, 0.1f, 100.0f);
}