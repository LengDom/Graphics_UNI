#version 410

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 colour_in;

out vec3 colour_vert;
uniform mat4 transform;

void main()
{
    // homogeneous coordinate
    gl_Position = transform * vec4(pos, 1.0);
    colour_vert = colour_in;
}