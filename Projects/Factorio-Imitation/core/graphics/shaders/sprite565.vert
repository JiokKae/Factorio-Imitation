#version 330

layout(std140) uniform vsConstants
{
    mat4 projection;
} _19;

layout(location = 0) in vec3 position;
out vec2 vUV;
layout(location = 1) in vec2 uv;
out vec4 vTintWith565Multiplier;
layout(location = 2) in vec4 tint;

void main()
{
    gl_Position = _19.projection * vec4(position, 1.0);
    vUV = uv;
    vTintWith565Multiplier = vec4(tint.xyz * vec3(1.5751007595099508762359619140625e-05, 0.0004960317746736109256744384765625, 0.0322580635547637939453125), tint.w);
}

