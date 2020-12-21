#version 330

layout(std140) uniform vsConstants
{
    mat4 projection;
} _19;

layout(location = 0) in vec2 position;
out vec4 vColor;
layout(location = 1) in vec4 color;

void main()
{
    gl_Position = _19.projection * vec4(position, 0.0, 1.0);
    vColor = color;
}

