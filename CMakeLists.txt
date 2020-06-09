cmake_minimum_required(VERSION 3.10)
project(GLTF_ANIMATION)

# add build property
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra -Wno-unused-parameter -Wno-implicit-fallthrough")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()

IF (CMAKE_BUILD_TYPE STREQUAL Debug)
    ADD_DEFINITIONS(-DDEBUG)
ENDIF ()

MESSAGE(STATUS "Build type: ${CMAKE_BUILD_TYPE}")


include_directories("${PROJECT_SOURCE_DIR}/external/include")
include_directories("${PROJECT_SOURCE_DIR}/external/include/orca")
include_directories("${PROJECT_SOURCE_DIR}/external/include/tinyglTF")

# OpenGL Dependence
include_directories(/usr/local/include/)
link_directories(/usr/local/lib/)
find_package(OpenGL REQUIRED)

# add main executable program
include_directories("${PROJECT_SOURCE_DIR}/src/src")
aux_source_directory(src/src/Camera     CAMERA_FILES)
aux_source_directory(src/src/Keyboard   KEYBOARD_FILES)
aux_source_directory(src/src/Model      MODEL_FILES)
aux_source_directory(src/src/Mouse      MOUSE_FILES)
aux_source_directory(src/src/Shader     SHADER_FILES)
set(SOURCE_FILES ${CAMERA_FILES} ${KEYBOARD_FILES} ${MODEL_FILES} ${MOUSE_FILES} ${SHADER_FILES} 
                src/main.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# link sub cmakelists
TARGET_LINK_LIBRARIES(${PROJECT_NAME} glfw)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} GL)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} GLEW)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} dl)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} pthread)