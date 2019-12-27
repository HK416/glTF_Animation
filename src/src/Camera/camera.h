/*************************/
/*  FILE NAME: camera.h  */
/*************************/
#ifndef _CAMERA_H_
#define _CAMERA_H_

/**************/
/*  INCLUDES  */
/**************/
#include <vector.hpp>
#include <matrix.hpp>

/*************************************/
/*  ENUM CLASS NAME: CameraMovement  */
/*************************************/
enum class CameraMovement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
}; // enum class CameraMovement

/**********************************/
/*  ENUM CLASS NAME: CameraSpeed  */
/**********************************/
enum class Speed
{
    SPEED_DOWN = -1,
    SPEED_UP = 1
}; // enum class CameraSpeed

/************************/
/*  CLASS NAME: Camera  */
/************************/
class Camera
{
private:
    inline static constexpr float mouse_sensitivity = 0.1f;

public:
    Camera();
    Camera(const Camera& other);

public:
    int GetScreenWidth() const;
    int GetScreenHeight() const;
    orca::mat4<float> GetViewMatrix() const;
    orca::mat4<float> GetProjectionMatrix() const;
    void ScreenResize(int width, int height);
    void MouseScroll(float x_offset, float y_offset);
    void MouseMovement(float x_offset, float y_offset);
    void CameraTranslation(CameraMovement movement, float delta_time);
    void CameraSpeed(Speed speed);

private:
    orca::vec3<float> position;
    orca::vec3<float> direction;
    orca::vec3<float> up;

    float yaw;
    float pitch;
    float roll;

    int screen_width;
    int screen_height;
    float fovy;

    float camera_speed;
}; // class Camera
#endif // !_CAMERA_H_