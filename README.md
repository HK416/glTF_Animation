# glTF_Animation
 3D animation viewer for glTF format files using `TinyglTF` library

# Program description
 This program reads a file in glTF format and outputs the data to the screen using OpenGL.
 if there is animation data, only the first animation is shown on the screen.
 This code is based on `SacchaWillems' Vulkan-glTF-PBR`.
 The comment line has been translated using Google Translator.
 
 you can move with the W, A, S, D, left Shift, Space keys.
 you can adjust the moving speed with the Q and E keys.
 you can rotate the mouse by moving it.
 you can zoom using the scroll of the mouse.

# Build test environment
 Windows10, gcc, x64, std=c++17

# Library used
 - GLFW (3.3 ver)    [url: https://github.com/glfw/glfw]
 - GLEW (2.1.0 ver)  [url: https://github.com/nigels-com/glew]
 - tinyglTF          [url: https://github.com/syoyo/tinygltf]

 # Reference
 - SacchaWillems' Vulkan-glTF-PBR [url: https://github.com/SaschaWillems/Vulkan-glTF-PBR]
 - gltf Tutorial                  [url: https://github.com/KhronosGroup/glTF-Tutorials/blob/master/gltfTutorial/gltfTutorial_020_Skins.md]