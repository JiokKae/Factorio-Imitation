#version 330

layout(std140) uniform fsConstants
{
    mat4 colorMatrix;
    float brightness;
    float contrast;
    float saturation;
    float factor;
    float summand;
    int flags;
} _130;

uniform sampler2D source;

in vec2 vUV;
layout(location = 0) out vec4 fragColor;

mat3 saturationMatrix(float saturation)
{
    vec3 luminance = vec3(0.308600008487701416015625, 0.609399974346160888671875, 0.08200000226497650146484375);
    float oneMinusSat = 1.0 - saturation;
    vec3 red = vec3(luminance.x * oneMinusSat);
    red.x += saturation;
    vec3 green = vec3(luminance.y * oneMinusSat);
    green.y += saturation;
    vec3 blue = vec3(luminance.z * oneMinusSat);
    blue.z += saturation;
    return mat3(vec3(red), vec3(green), vec3(blue));
}

void brightnessAdjust(inout vec4 color, float b)
{
    vec3 _98 = pow(max(vec3(0.0), color.xyz), vec3(1.0 - b));
    color = vec4(_98.x, _98.y, _98.z, color.w);
}

void contrastAdjust(inout vec4 color, float c)
{
    float t = 0.5 - (c * 0.5);
    vec3 _112 = (color.xyz * c) + vec3(t);
    color = vec4(_112.x, _112.y, _112.z, color.w);
}

void main()
{
    vec4 color = texture(source, vUV);
    vec3 _145 = clamp(_130.colorMatrix * vec4(color.xyz, 1.0), vec4(0.0), vec4(1.0)).xyz;
    color = vec4(_145.x, _145.y, _145.z, color.w);
    if (_130.saturation != 1.0)
    {
        float param = _130.saturation;
        vec3 _162 = saturationMatrix(param) * color.xyz;
        color = vec4(_162.x, _162.y, _162.z, color.w);
    }
    vec4 param_1 = color;
    float param_2 = _130.brightness;
    brightnessAdjust(param_1, param_2);
    color = param_1;
    vec4 param_3 = color;
    float param_4 = _130.contrast;
    contrastAdjust(param_3, param_4);
    color = param_3;
    fragColor = color;
}

