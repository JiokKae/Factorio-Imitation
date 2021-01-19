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
} _150;

uniform sampler2D waterNoiseTexture;
uniform sampler2D waterMaskTexture;

in vec2 vUV;
in vec4 vColor;
layout(location = 0) out vec4 fragColor;

float noise_func(vec2 uv)
{
    return texture(waterNoiseTexture, uv).x;
}

vec2 mul(vec2 v, mat2 m)
{
    return v * m;
}

vec2 rotate(inout vec2 uv)
{
    vec2 param = uv * 2.0;
    uv += vec2(noise_func(param) * 0.0199999995529651641845703125);
    float angle = 3.0;
    float sinRot = sin(angle);
    float cosRot = cos(angle);
    mat2 rotation = mat2(vec2(cosRot, -sinRot), vec2(sinRot, cosRot));
    vec2 param_1 = uv;
    mat2 param_2 = rotation;
    return mul(param_1, param_2);
}

float fbm(inout vec2 uv, float t)
{
    float f = 0.0;
    float total = 0.0;
    float mul_1 = 1.0;
    for (int i = 0; i < 3; i++)
    {
        vec2 param = uv + vec2((t * 0.0024999999441206455230712890625) * (1.0 - mul_1));
        f += (noise_func(param) * mul_1);
        total += mul_1;
        vec2 param_1 = uv * 1.10000002384185791015625;
        vec2 _125 = rotate(param_1);
        uv = _125;
        mul_1 *= 0.75;
    }
    return f / total;
}

vec4 MainPS()
{
    vec2 uv = vUV;
    uv.y *= 1.414000034332275390625;
    vec2 param = vec2(sin(_150.time * _150.animationSpeed) * _150.animationScale) + uv;
    float param_1 = _150.time;
    float _170 = fbm(param, param_1);
    float value = _170 + 0.100000001490116119384765625;
    vec3 mask = texelFetch(waterMaskTexture, ivec2(gl_FragCoord.xy), 0).xyz;
    float darks = 1.0 - ceil(value + _150.darkThreshold);
    float reflection = smoothstep(0.0, _150.reflectionThreshold, ((1.0 - (value * 0.800000011920928955078125)) - 0.60000002384185791015625) + (mask.x * 0.25));
    float specular = ceil((value + _150.specularThreshold) - mask.x);
    vec4 color = vec4(vColor.xyz * ((value + (specular * 0.189999997615814208984375)) - (mask.z * 0.300000011920928955078125)), vColor.w);
    color = mix(color, _150.specularLightness, vec4((reflection * 0.64999997615814208984375) + (darks * 0.10999999940395355224609375)));
    color *= (1.0 - mask.y);
    color = mix(color, _150.foamColor, vec4(smoothstep(-0.3499999940395355224609375, 0.300000011920928955078125, mask.z - (value * 2.150000095367431640625))));
    return color;
}

void main()
{
    fragColor = MainPS();
}

