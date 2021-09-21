#version 330

layout(std140) uniform vsConstants
{
    mat4 projection;
} _19;

layout(location = 0) in vec3 position;
out vec2 vUV;
layout(location = 1) in vec2 uv;
out vec4 vTint;
layout(location = 2) in vec4 tint;
flat out uint vExtra;
layout(location = 3) in uint extra;

void main()
{
    gl_Position = _19.projection * vec4(position, 1.0);
    vUV = uv;
    vTint = tint;
    vExtra = extra;
}

