#version 330

layout(location = 0) in vec3 position;
out vec2 vUV;
layout(location = 1) in vec2 uv;

void main()
{
    gl_Position = vec4(position, 1.0);
    vUV = uv;
}

