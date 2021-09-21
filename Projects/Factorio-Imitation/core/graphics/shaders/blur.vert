#version 330

layout(std140) uniform blurConstants
{
    mat4 projection;
    mat4 model;
    vec2 regionStart;
    vec2 regionSize;
    vec4 coeff0;
    vec4 coeff1234;
    vec2 direction;
} _20;

layout(location = 0) in vec2 position;
out vec2 vUV;
layout(location = 1) in vec2 uv;

void main()
{
    gl_Position = (_20.projection * _20.model) * vec4(position, 0.0, 1.0);
    vUV = _20.regionStart + (uv * _20.regionSize);
}

