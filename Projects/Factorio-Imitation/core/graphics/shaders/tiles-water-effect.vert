#version 330

layout(std140) uniform vsConstants
{
    mat4 projection;
} _30;

out vec4 vColor;
layout(location = 3) in vec4 color;
out vec2 vUV;
layout(location = 1) in vec2 uv;
layout(location = 0) in vec2 position;
layout(location = 2) in uvec2 masks;

void main()
{
    vColor = color;
    vUV = uv;
    gl_Position = _30.projection * vec4(position, 0.0, 1.0);
}

