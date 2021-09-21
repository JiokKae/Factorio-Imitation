#version 330

layout(std140) uniform vsConstants
{
    mat4 projection;
} _19;

layout(location = 0) in vec3 position;
out vec2 vUV1;
layout(location = 1) in vec2 uv1;
out vec2 vUV2;
out vec4 vTint;
layout(location = 2) in vec4 tint;

void main()
{
    gl_Position = _19.projection * vec4(position, 1.0);
    vUV1 = uv1;
    vUV2 = uv1;
    vTint = tint;
}

