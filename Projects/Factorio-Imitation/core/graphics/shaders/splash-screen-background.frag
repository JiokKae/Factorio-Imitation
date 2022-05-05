#version 330

layout(std140) uniform fsConstants
{
    float width;
    float height;
    float minIntensity;
    float maxIntensity;
    float uMul;
    float vMul;
} _18;

in vec2 vUV;
layout(location = 0) out vec4 fragColor;

void main()
{
    vec2 a = (vUV - vec2(0.5)) * vec2(_18.uMul, _18.vMul);
    float d = length(a) * 2.0;
    d = clamp(1.0 - d, 0.0, 1.0);
    d = mix(_18.minIntensity, _18.maxIntensity, d);
    vec3 vDither = vec3(dot(vec2(171.0, 231.0), vUV * vec2(_18.width, _18.height)));
    vDither = fract(vDither / vec3(103.0, 71.0, 97.0)) - vec3(0.5);
    fragColor = vec4((vDither / vec3(196.0)) + vec3(d), 1.0);
}

