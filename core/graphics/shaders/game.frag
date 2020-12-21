#version 330

layout(std140) uniform fsConstants
{
    vec2 zoom_to_world_params;
    float timer;
    float lutSize;
    vec2 resolution;
    float lutAlpha;
    float lightMul;
    float lightAdd;
    uint debugShowLut;
} _70;

uniform sampler2D gameview;
uniform sampler3D lut1;
uniform sampler2D lightmap;

in vec2 vUV;
layout(location = 0) out vec4 fragColor;

vec3 colorToLut16Index(vec3 inputColor)
{
    return (inputColor * 0.9375) + vec3(0.03125);
}

void main()
{
    vec2 uv = vUV;
    vec4 color = texture(gameview, uv);
    vec3 param = color.xyz;
    vec3 lookupIndex = colorToLut16Index(param);
    vec4 sunlitColor = vec4(textureLod(lut1, lookupIndex, 0.0).xyz, color.w);
    vec4 light = texture(lightmap, uv);
    vec3 _82 = (light.xyz * vec3(_70.lightMul)) + vec3(_70.lightAdd);
    light = vec4(_82.x, _82.y, _82.z, light.w);
    vec4 finalColor = mix(sunlitColor, color, light);
    fragColor = finalColor;
}

