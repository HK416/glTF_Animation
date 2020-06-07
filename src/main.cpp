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
#include <memory>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <stdexcept>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Model/model.h"
#include "Mouse/mouse.h"
#include "Camera/camera.h"
#include "Shader/shader.h"
#include "Keyboard/keyboard.h"

/***************/
/*  CONSTANTS  */
/***************/
#ifdef _WIN32
constexpr char* ANIM_VERT_SHADER = "\\GLSL\\anim_vert.glsl";
constexpr char* DEF_VERT_SHADER = "\\GLSL\\def_vert.glsl";
constexpr char* ANIM_FRAG_SHADER = "\\GLSL\\anim_frag.glsl";
constexpr char* DEF_FRAG_SHADER = "\\GLSL\\def_frag.glsl";
#else
constexpr char* ANIM_VERT_SHADER = "/GLSL/vert_animation.glsl";
constexpr char* DEF_VERT_SHADER = "/GLSL/vert_default.glsl";
constexpr char* ANIM_FRAG_SHADER = "/GLSL/frag.glsl";
constexpr char* DEF_FRAG_SHADER = "/GLSL/frag.glsl";
#endif
constexpr char* TITLE = "glTF Animation Application";
constexpr int WIDTH = 1280;
constexpr int HEIGHT = 720;

/*************/
/*  GLOBALS  */
/*************/
GLFWwindow* window;
Keyboard keyboard;
Camera camera;
Mouse mouse;

Shader* curr_shader;
std::unique_ptr<Shader> anim_shader;
std::unique_ptr<Shader> def_shader;
int projection_location;
int view_location;

std::unique_ptr<Model> model;

std::string program_dir;
std::string program_name;
std::string model_dir;
std::string model_name;

/*************************/
/*  FUNCTION PROTOTYPES  */
/*************************/
void initialize(int, char**);
void cleanup();
void run();

void inputHandling();
void update(double delta_time);
void render();

void getFileDirAndName(const std::string&, std::string*, std::string*);

static void errorCallback(int, const char*);
static void keyboardCallback(GLFWwindow*, int, int, int, int);
static void mousePosCallback(GLFWwindow*, double, double);
static void mouseScrollCallback(GLFWwindow*, double, double);

/**********/
/*  MAIN  */
/**********/
int main(int argc, char* argv[])
{
    try
    {
        initialize(argc, argv);
        run();
        cleanup();
        return EXIT_SUCCESS;
    }
    catch(const std::exception& e)
    {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        cleanup();
        system("pause");
        return EXIT_FAILURE;
    }
}

void initialize(int argc, char** argv)
{
    if (argc != 2)
        throw std::runtime_error("Usage: " + std::string(argv[0]) + " <glTF File>");

    getFileDirAndName(argv[0], &program_dir, &program_name);
    getFileDirAndName(argv[1], &model_dir, &model_name);


    // set GLFW error callback function 
    glfwSetErrorCallback(errorCallback);

    // initialize GLFW
    if (!glfwInit())
        throw std::runtime_error("Failed to initialize GLFW.");

    // set GLFW configuration
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    // create GLFW window instance 
    window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);
    if (!window)
        throw std::runtime_error("Failed to create GLFW window instance.");
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window, keyboardCallback);
    glfwSetScrollCallback(window, mouseScrollCallback);
    glfwSetCursorPosCallback(window, mousePosCallback);

    // initialize GLEW 
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
        throw std::runtime_error("Failed to initialize GLEW.");



    std::cout << "Model Loading...";
    model = std::make_unique<Model>(model_dir, model_name);
    std::cout << "Success!" << std::endl;
    std::cout << std::endl;

    def_shader = std::make_unique<Shader>(program_dir + DEF_VERT_SHADER, program_dir + DEF_FRAG_SHADER);
    anim_shader = std::make_unique<Shader>(program_dir + ANIM_VERT_SHADER, program_dir + ANIM_FRAG_SHADER);

    curr_shader = model->IsAnimated() ? anim_shader.get() : def_shader.get();
    projection_location = glGetUniformLocation(curr_shader->get(), "projection");
    view_location = glGetUniformLocation(curr_shader->get(), "view");


    glEnable(GL_DEPTH_TEST);
}

void cleanup()
{
    anim_shader.reset();
    def_shader.reset();
    model.reset();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void run()
{
    std::cout << "[File Infomation]" << std::endl;
    std::cout << "Program Directory: " << program_dir << std::endl;
    std::cout << "Program Name: " << program_name << std::endl;
    std::cout << "Model File Directory: " << model_dir << std::endl;
    std::cout << "Model File Name: " << model_name << std::endl;
    std::cout << std::endl;

    std::cout << "[OpenGL Infomation]" << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << std::endl;

    double prev_time;
    double curr_time;
    double delta_time;
    double frame_late;

    prev_time = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        curr_time = glfwGetTime();
        delta_time = curr_time - prev_time;
        frame_late = 1.0 / delta_time;
        prev_time = curr_time;

        inputHandling();
        update(delta_time);
        render();
    }
}

void inputHandling()
{
    glfwPollEvents();

    if (keyboard.isKeyDown(KEY_ESCAPE))
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void update(double delta_time)
{
    model->Update(delta_time);

    camera.speed += 1.0 * keyboard.isKeyDown(KEY_E);
    camera.speed -= 1.0 * keyboard.isKeyDown(KEY_Q);
    camera.speed = std::max(camera.speed, 1.0);

    float velocity = camera.speed * static_cast<float>(delta_time);
    orca::vec3f front = orca::Normalize(10.0f * orca::vec3<float>(camera.dir.x, 0.0f, camera.dir.z));
    orca::vec3f left = orca::Normalize(10.0f * orca::Cross(camera.up, front));
    orca::vec3f up = orca::Normalize(camera.up);

    camera.pos += front * velocity * keyboard.isKeyDown(KEY_W);
    camera.pos -= front * velocity * keyboard.isKeyDown(KEY_S);
    camera.pos += left * velocity * keyboard.isKeyDown(KEY_D);
    camera.pos -= left * velocity * keyboard.isKeyDown(KEY_A);
    camera.pos += up * velocity * keyboard.isKeyDown(KEY_SPACE);
    camera.pos -= up * velocity * keyboard.isKeyDown(KEY_LEFT_SHIFT);

    glUseProgram(curr_shader->get());
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, camera.getProjectionMatrix(WIDTH, HEIGHT));
    glUniformMatrix4fv(view_location, 1, GL_FALSE, camera.getViewMatrix());
    glUseProgram(NULL);
    
}

void render()
{
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(curr_shader->get());
    model->Render(curr_shader->get());
    glUseProgram(NULL);

    glfwSwapBuffers(window);
}

// this function gets the directory and the name of a file from a string
void getFileDirAndName(const std::string& str, std::string* dir, std::string* name)
{
    (*dir).clear();
    (*name).clear();
#ifdef _WIN32
    size_t pos = str.rfind('\\', str.length());
#else
    size_t pos = str.rfind('/', str.length());
#endif
    if (pos != std::string::npos)
    {
        (*dir) = str.substr(0, pos);
        (*name) = str.substr(pos + 1, str.length());
    }
}

// this function handles GLFW error messages.
static void errorCallback(int error_code, const char* error_description)
{
    std::cerr << "<ERROR CODE> " << error_code << ", <INFO> " << error_description << std::endl;
}

// this function runs every time you press the keyboard
static void keyboardCallback(GLFWwindow* window, int key, int scan_code, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        keyboard.keyDown(KEY_ESCAPE);
    else if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
        keyboard.keyUp(KEY_ESCAPE);

    else if (key == GLFW_KEY_A && action == GLFW_PRESS)
        keyboard.keyDown(KEY_A);
    else if (key == GLFW_KEY_A && action == GLFW_RELEASE)
        keyboard.keyUp(KEY_A);

    else if (key == GLFW_KEY_D && action == GLFW_PRESS)
        keyboard.keyDown(KEY_D);
    else if (key == GLFW_KEY_D && action == GLFW_RELEASE)
        keyboard.keyUp(KEY_D);

    else if (key == GLFW_KEY_W && action == GLFW_PRESS)
        keyboard.keyDown(KEY_W);
    else if (key == GLFW_KEY_W && action == GLFW_RELEASE)
        keyboard.keyUp(KEY_W);

    else if (key == GLFW_KEY_S && action == GLFW_PRESS)
        keyboard.keyDown(KEY_S);
    else if (key == GLFW_KEY_S && action == GLFW_RELEASE)
        keyboard.keyUp(KEY_S);

    else if (key == GLFW_KEY_Q && action == GLFW_PRESS)
        keyboard.keyDown(KEY_Q);
    else if (key == GLFW_KEY_Q && action == GLFW_RELEASE)
        keyboard.keyUp(KEY_Q);

    else if (key == GLFW_KEY_E && action == GLFW_PRESS)
        keyboard.keyDown(KEY_E);
    else if (key == GLFW_KEY_E && action == GLFW_RELEASE)
        keyboard.keyUp(KEY_E);

    else if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
        keyboard.keyDown(KEY_LEFT_SHIFT);
    else if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
        keyboard.keyUp(KEY_LEFT_SHIFT);

    else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        keyboard.keyDown(KEY_SPACE);
    else if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
        keyboard.keyUp(KEY_SPACE); 
}

// this function executes whenever the mouse moves
static void mousePosCallback(GLFWwindow* window, double x_pos, double y_pos)
{
    if (mouse.first_input)
    {
        mouse.x_pos = x_pos;
        mouse.y_pos = y_pos;
        mouse.first_input = false;
        return;
    }

    double x_offset = (mouse.x_pos - x_pos) * mouse.sensitivity;
    double y_offset = (mouse.y_pos - y_pos) * mouse.sensitivity;

    mouse.x_pos = x_pos;
    mouse.y_pos = y_pos;

    camera.yaw += x_offset;
    camera.pitch = std::clamp(camera.pitch + y_offset, -89.9, 89.9);

    camera.dir.x = cos(orca::DegreeToRadian(camera.yaw)) * cos(orca::DegreeToRadian(camera.pitch));
    camera.dir.y = sin(orca::DegreeToRadian(camera.pitch));
    camera.dir.z = sin(orca::DegreeToRadian(camera.yaw)) * cos(orca::DegreeToRadian(camera.pitch));
}

// this function executes whenever scroll the mouse
static void mouseScrollCallback(GLFWwindow* window, double x_offset, double y_offset)
{
    camera.fovy = std::clamp(camera.fovy - y_offset, 1.0, 45.0);
}  