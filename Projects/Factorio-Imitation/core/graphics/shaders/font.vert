#version 330

layout(std140) uniform vsConstants
{
    mat4 projection;
} _19;

layout(location = 0) in vec2 position;
out vec2 vUV;
layout(location = 1) in vec2 uv;
flat out vec4 vColor;
layout(location = 2) in vec4 color;
flat out vec4 vBorderColor;
layout(location = 3) in vec4 borderColor;

void main()
{
    gl_Position = _19.projection * vec4(position, 0.0, 1.0);
    vUV = uv;
    vColor = color;
    vBorderColor = borderColor;
}

