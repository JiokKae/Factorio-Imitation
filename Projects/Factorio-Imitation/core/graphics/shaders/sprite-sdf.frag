#version 330

layout(std140) uniform fsConstants
{
    vec4 globalColor;
    vec2 atlasSize;
    float minDist;
    float maxDist;
    float radius;
    float sharpness;
    float inset;
} _30;

uniform sampler2D tex;

in vec2 vUV1;
layout(location = 0) out vec4 fragColor;
in vec4 vTint;
in vec2 vUV2;

void main()
{
    float dist = texture(tex, vUV1).x;
    fragColor = vTint * smoothstep(_30.minDist, _30.maxDist, dist);
}

