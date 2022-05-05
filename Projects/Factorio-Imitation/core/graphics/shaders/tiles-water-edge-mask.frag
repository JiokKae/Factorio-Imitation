#version 330

layout(std140) uniform EffectUniforms
{
    vec4 specularLightness;
    vec4 foamColor;
    vec2 texSize;
    float animationSpeed;
    float animationScale;
    float darkThreshold;
    float reflectionThreshold;
    float specularThreshold;
    float time;
} _24;

uniform sampler2D atlasTexture;

in vec2 vUV;
in float vVariation;
layout(location = 0) out vec4 fragColor;
flat in uint vFlags;

void main()
{
    vec4 color = texture(atlasTexture, vUV);
    float wave = (sin((_24.time * 0.1500000059604644775390625) + vVariation) + 1.0) * 0.357142865657806396484375;
    color.y = smoothstep(wave, wave + 0.300000011920928955078125, color.y);
    fragColor = color;
}

