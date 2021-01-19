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
} _82;

uniform sampler2D gameview;
uniform sampler3D lut1;
uniform sampler2D lightmap;

in vec2 vUV;
layout(location = 0) out vec4 fragColor;

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

vec3 getnoise3(vec2 p)
{
    float param = p.x;
    float param_1 = p.y;
    float param_2 = floor(_82.timer / 3.0);
    return vec3(hash3(param, param_1, param_2));
}

vec3 colorToLut16Index(vec3 inputColor)
{
    return (inputColor * 0.9375) + vec3(0.03125);
}

void main()
{
    vec2 uv = vUV;
    vec4 color = texture(gameview, uv);
    vec2 param = uv;
    vec3 _noise = getnoise3(param);
    vec3 _133 = color.xyz + ((_noise * _82.zoom_to_world_params.x) + vec3(_82.zoom_to_world_params.y));
    color = vec4(_133.x, _133.y, _133.z, color.w);
    vec3 param_1 = color.xyz;
    vec3 lookupIndex = colorToLut16Index(param_1);
    vec4 sunlitColor = vec4(textureLod(lut1, lookupIndex, 0.0).xyz, color.w);
    vec4 light = texture(lightmap, uv);
    vec3 _174 = (light.xyz * vec3(_82.lightMul)) + vec3(_82.lightAdd);
    light = vec4(_174.x, _174.y, _174.z, light.w);
    vec4 finalColor = mix(sunlitColor, color, light);
    fragColor = finalColor;
}

