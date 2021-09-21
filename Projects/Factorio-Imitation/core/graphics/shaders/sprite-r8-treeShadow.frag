#version 330

layout(std140) uniform fsConstants
{
    vec2 invShadowTexSize;
    float time;
    float inverseExpectedUVDerivativeTimesStrength;
} _112;

uniform sampler2D tex;

layout(location = 0) out vec4 fragColor;
in vec2 vUV;
in vec4 vTint;

float hmix(float a, float b)
{
    return fract(sin((a * 12.98980045318603515625) + b) * 43758.546875);
}

float hash3(float a, float b, float c)
{
    float param = a;
    float param_1 = b;
    float ab = hmix(param, param_1);
    float param_2 = a;
    float param_3 = c;
    float ac = hmix(param_2, param_3);
    float param_4 = b;
    float param_5 = c;
    float bc = hmix(param_4, param_5);
    float param_6 = ac;
    float param_7 = bc;
    float param_8 = ab;
    float param_9 = hmix(param_6, param_7);
    return hmix(param_8, param_9);
}

float getnoise(vec2 p, float time)
{
    float param = p.x;
    float param_1 = p.y;
    float param_2 = time;
    return hash3(param, param_1, param_2);
}

void main()
{
    fragColor = vTint * texture(tex, vUV).x;
    vec2 localUV = vUV * vec2(textureSize(tex, 0));
    vec2 param = floor(localUV);
    float param_1 = floor(_112.time);
    vec2 param_2 = floor(localUV.yx);
    float param_3 = floor(_112.time);
    vec2 past = vec2(getnoise(param, param_1), getnoise(param_2, param_3));
    vec2 param_4 = floor(localUV);
    float param_5 = ceil(_112.time);
    vec2 param_6 = floor(localUV.yx);
    float param_7 = ceil(_112.time);
    vec2 next = vec2(getnoise(param_4, param_5), getnoise(param_6, param_7));
    vec2 offset = (mix(past, next, vec2(fract(_112.time))) * 2.0) - vec2(1.0);
    offset *= (dFdx(localUV.x) * _112.inverseExpectedUVDerivativeTimesStrength);
    fragColor = vTint * texture(tex, (localUV + offset) * _112.invShadowTexSize).x;
}

