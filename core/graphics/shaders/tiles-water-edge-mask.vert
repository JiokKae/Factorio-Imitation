#version 330

layout(std140) uniform vsConstants
{
    mat4 projection;
} _42;

flat out uint vFlags;
layout(location = 3) in uint flags;
out float vVariation;
out vec2 vUV;
layout(location = 1) in vec2 uv;
layout(location = 0) in vec2 position;
layout(location = 2) in uvec2 masks;

void main()
{
    vFlags = flags;
    vVariation = (0.875 + (float(flags >> uint(16)) * 0.0078125)) * 100.0;
    vUV = uv;
    gl_Position = _42.projection * vec4(position, 0.0, 1.0);
}

