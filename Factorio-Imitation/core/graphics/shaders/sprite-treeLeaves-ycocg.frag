#version 330

layout(std140) uniform fsConstants
{
    vec2 invLeafTexSize;
    vec2 strength;
    vec2 distort;
    vec2 speed;
    float time;
    float inverseExpectedUVDerivative;
    uint debug;
} _98;

uniform sampler2D texDistortion;
uniform sampler2D texYCoCg;
uniform sampler2D texAlpha;

in vec2 vUVTexture;
in vec2 vUVDistortion;
in vec4 vTint;
layout(location = 0) out vec4 fragColor;

vec4 YCoCgToRGB(vec4 ycocg, float alpha)
{
    float Y = ycocg.w;
    float scale = 1.0 / ((31.875 * ycocg.z) + 1.0);
    float Co = (ycocg.x - 0.501960813999176025390625) * scale;
    float Cg = (ycocg.y - 0.501960813999176025390625) * scale;
    float R = (Y + Co) - Cg;
    float G = Y + Cg;
    float B = (Y - Co) - Cg;
    return vec4(R, G, B, alpha);
}

void main()
{
    vec2 localUV = vUVTexture;
    vec2 distortion = vec2(0.0);
    vec4 offset = vec4(0.0);
    if (any(notEqual(vUVDistortion, vec2(-1.0))))
    {
        offset = texture(texDistortion, vUVDistortion);
        float localTime = _98.time + (((vTint.x + vTint.y) + vTint.z) * 5.0);
        distortion.x = offset.x * sin((offset.y * _98.distort.x) + (localTime * _98.speed.x));
        distortion.y = offset.x * cos((offset.y * _98.distort.y) + (localTime * _98.speed.y));
        vec2 localPixelXY = localUV * vec2(textureSize(texYCoCg, 0));
        distortion *= (dFdx(localPixelXY.x) * _98.inverseExpectedUVDerivative);
        vec2 copy = localPixelXY;
        localUV = (floor(localPixelXY) + (distortion * _98.strength)) * _98.invLeafTexSize;
        distortion = floor(localUV * vec2(textureSize(texYCoCg, 0))) - floor(copy);
        if (_98.debug != 0u)
        {
            fragColor = vec4(0.5 + (0.20000000298023223876953125 * distortion.x), 0.5 + (0.20000000298023223876953125 * distortion.y), dFdx(localPixelXY.x) * _98.inverseExpectedUVDerivative, 0.0);
        }
    }
    float alpha = texture(texAlpha, localUV).x;
    bool _228 = !(_98.debug != 0u);
    bool _235;
    if (!_228)
    {
        _235 = all(equal(vUVDistortion, vec2(-1.0)));
    }
    else
    {
        _235 = _228;
    }
    if (_235)
    {
        vec4 yCoCg = texture(texYCoCg, localUV);
        vec4 param = yCoCg;
        float param_1 = alpha;
        fragColor = YCoCgToRGB(param, param_1);
        vec3 grayscale = ((fragColor.xxx + fragColor.yyy) + fragColor.zzz) / vec3(4.0);
        vec3 _270 = mix(grayscale, fragColor.xyz, vec3(vTint.w)) * vTint.xyz;
        fragColor = vec4(_270.x, _270.y, _270.z, fragColor.w);
    }
    else
    {
        vec3 _277 = fragColor.xyz * alpha;
        fragColor = vec4(_277.x, _277.y, _277.z, fragColor.w);
        fragColor.w = alpha;
    }
}

