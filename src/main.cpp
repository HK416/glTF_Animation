/*************************/
/*  FILE NAME: main.cpp  */
/*************************/

/**
 * Program description
 * This program reads a file in glTF format and outputs the data to the screen using OpenGL.
 * if there is animation data, only the first animation is shown on the screen.
 * This code is based on SacchaWillems' Vulkan-glTF-PBR.
 * The comment line has been translated using Google Translator.
 * 
 * you can move with the W, A, S, D, left Shift, Space keys.
 * you can adjust the moving speed with the Q and E keys.
 * you can rotate the mouse by moving it.
 * you can zoom using the scroll of the mouse.
 * 
 * Build environment
 * Windows10, gcc, x64, std=c++17
 * 
 * Library used
 * 1. GLFW (3.3 ver)   [url: https://github.com/glfw/glfw]
 * 2. GLEW (2.1.0 ver) [url: https://github.com/nigels-com/glew]
 * 3. tinyglTF         [url: https://github.com/syoyo/tinygltf]
 * 
 * Refernce
 * 1. SacchaWillems' Vulkan-glTF-PBR [url: https://github.com/SaschaWillems/Vulkan-glTF-PBR]
 * 2. gltf Tutorial                  [url: https://github.com/KhronosGroup/glTF-Tutorials/blob/master/gltfTutorial/gltfTutorial_020_Skins.md]
 */

/**************/
/*  INCLUDES  */
/**************/
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "src/Camera/camera.h"
#include "src/Shader/shader.h"
#include "src/Model/model.h"

/**********************/
/*  GLOBAL VARIABLES  */
/**********************/
constexpr char* application_name = "glTF Animation Application";
Camera camera;
double mouse_pos_x;
double mouse_pos_y;
bool first_mouse_input = true;

/*************************/
/*  FUNCTION PROTOTYPES  */
/*************************/
void GLFWResizeCallback(GLFWwindow* glfw_window, int width, int height);
void GLFWMouseCallback(GLFWwindow* glfw_window, double x_pos, double y_pos);
void GLFWScrollCallback(GLFWwindow* glfw_window, double x_offset, double y_offset);
void KeyInputFunc(GLFWwindow* glfw_window, float delta_time);

/**********/
/*  MAIN  */
/**********/
int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <glTF File>" << std::endl;
        system("pause");
        return -1;
    }

    /* initialize GLFW */
    if(glfwInit() == false)
    {
        std::cout << "Failed to initialize GLFW." << std::endl;
        return -1;
    }

    /* set GLFW configuration */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__

    /* create GLFW window instance */
    GLFWwindow* glfw_window = glfwCreateWindow(camera.GetScreenWidth(),
                                                camera.GetScreenHeight(),
                                                application_name, 
                                                nullptr, 
                                                nullptr);
    if(glfw_window == nullptr)
    {
        std::cout << "Failed to create GLFW window instance." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(glfw_window);
    glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(glfw_window, GLFWResizeCallback);
    glfwSetCursorPosCallback(glfw_window, GLFWMouseCallback);
    glfwSetScrollCallback(glfw_window, GLFWScrollCallback);

    /* initialize GLEW */
    glewExperimental = true;
    if(glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW." << std::endl;
        glfwDestroyWindow(glfw_window);
        glfwTerminate();
        return -1;
    }
    glEnable(GL_DEPTH_TEST);
    
    std::cout << "Loading Model..." << std::endl;
    Model model(argv[1]);
    model.SetupModel();

    Shader* curr_shader = nullptr;
    Shader def_shader("./GLSL/vert_default.glsl", "./GLSL/frag.glsl");
    Shader anim_shader("./GLSL/vert_animation.glsl", "./GLSL/frag.glsl");
    if(model.IsAnimated() == true)
        curr_shader = &anim_shader;
    else
        curr_shader = &def_shader;

    /* render loop */
    float prev_time = glfwGetTime();
    while(glfwWindowShouldClose(glfw_window) == 0)
    {
        float current_time = glfwGetTime();
        float delta_time = current_time - prev_time;
        prev_time = current_time;

        /* update */
        {
            glfwPollEvents();
            KeyInputFunc(glfw_window, delta_time);
            model.Update(delta_time);
        }
    
        /* render */
        /* TODO: some models require rotation. add the code needed to rotate the model. */
        {
            glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            glUseProgram(curr_shader->get());
            glUniformMatrix4fv(glGetUniformLocation(curr_shader->get(), "projection"), 1, GL_FALSE, camera.GetProjectionMatrix());
            glUniformMatrix4fv(glGetUniformLocation(curr_shader->get(), "view"), 1, GL_FALSE, camera.GetViewMatrix());
            model.Render(curr_shader->get());
            
            glfwSwapBuffers(glfw_window);
        }
    }

    model.CleanupModel();

    /* destroy GLFW instance */
    glfwDestroyWindow(glfw_window);
    glfwTerminate();
    return 0;
}

/* this function executes whenever the window size changes */
void GLFWResizeCallback(GLFWwindow* glfw_window, int width, int height)
{
    camera.ScreenResize(width, height);
    glViewport(0, 0, width, height);
}

/* this function executes whenever the mouse moves */
void GLFWMouseCallback(GLFWwindow* glfw_window, double x_pos, double y_pos)
{
    if(first_mouse_input == true)
    {
        mouse_pos_x = x_pos;
        mouse_pos_y = y_pos;
        first_mouse_input = false;
        return;
    }

    double x_offset = mouse_pos_x - x_pos;
    double y_offset = mouse_pos_y - y_pos;

    mouse_pos_x = x_pos;
    mouse_pos_y = y_pos;

    camera.MouseMovement(x_offset, y_offset);
}

/* this function executes whenever scroll the mouse */
void GLFWScrollCallback(GLFWwindow* glfw_window, double x_offset, double y_offset)
{
    camera.MouseScroll(x_offset, y_offset);
}

/* run this function to handle keyboard input */
void KeyInputFunc(GLFWwindow* glfw_window, float delta_time)
{
    if(glfwGetKey(glfw_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(glfw_window, GLFW_TRUE);

    if(glfwGetKey(glfw_window, GLFW_KEY_W) == GLFW_PRESS)
        camera.CameraTranslation(CameraMovement::FORWARD, delta_time);
    if(glfwGetKey(glfw_window, GLFW_KEY_S) == GLFW_PRESS)
        camera.CameraTranslation(CameraMovement::BACKWARD, delta_time);
    if(glfwGetKey(glfw_window, GLFW_KEY_A) == GLFW_PRESS)
        camera.CameraTranslation(CameraMovement::LEFT, delta_time);
    if(glfwGetKey(glfw_window, GLFW_KEY_D) == GLFW_PRESS)
        camera.CameraTranslation(CameraMovement::RIGHT, delta_time);
    if(glfwGetKey(glfw_window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.CameraTranslation(CameraMovement::UP, delta_time);
    if(glfwGetKey(glfw_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.CameraTranslation(CameraMovement::DOWN, delta_time);
    if(glfwGetKey(glfw_window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.CameraSpeed(Speed::SPEED_DOWN);
    if(glfwGetKey(glfw_window, GLFW_KEY_E) == GLFW_PRESS)
        camera.CameraSpeed(Speed::SPEED_UP);
}
