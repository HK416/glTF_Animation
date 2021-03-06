#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord;
layout (location = 3) in uvec4 joint;
layout (location = 4) in vec4 weight;

uniform mat4 projection;
uniform mat4 view;

out VS_OUT
{
    vec2 texcoord;
} vs_out;

void main()
{
    vs_out.texcoord = texcoord;
    gl_Position = projection * view * vec4(position, 1.0);

} 