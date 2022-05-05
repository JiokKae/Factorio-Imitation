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
} _42;

uniform sampler2D texDistortion;
uniform sampler2D tex;

in vec2 vUVTexture;
in vec2 vUVDistortion;
in vec4 vTint;
layout(location = 0) out vec4 fragColor;

void main()
{
    vec2 localUV = vUVTexture;
    vec2 distortion = vec2(0.0);
    vec4 offset = vec4(0.0);
    if (any(notEqual(vUVDistortion, vec2(-1.0))))
    {
        offset = texture(texDistortion, vUVDistortion);
        float localTime = _42.time + (((vTint.x + vTint.y) + vTint.z) * 5.0);
        distortion.x = offset.x * sin((offset.y * _42.distort.x) + (localTime * _42.speed.x));
        distortion.y = offset.x * cos((offset.y * _42.distort.y) + (localTime * _42.speed.y));
        vec2 localPixelXY = localUV * vec2(textureSize(tex, 0));
        distortion *= (dFdx(localPixelXY.x) * _42.inverseExpectedUVDerivative);
        vec2 copy = localPixelXY;
        localUV = (floor(localPixelXY) + (distortion * _42.strength)) * _42.invLeafTexSize;
        distortion = floor(localUV * vec2(textureSize(tex, 0))) - floor(copy);
        if (_42.debug != 0u)
        {
            fragColor = vec4(0.5 + (0.20000000298023223876953125 * distortion.x), 0.5 + (0.20000000298023223876953125 * distortion.y), dFdx(localPixelXY.x) * _42.inverseExpectedUVDerivative, 0.0);
        }
    }
    bool _169 = !(_42.debug != 0u);
    bool _176;
    if (!_169)
    {
        _176 = all(equal(vUVDistortion, vec2(-1.0)));
    }
    else
    {
        _176 = _169;
    }
    if (_176)
    {
        fragColor = texture(tex, localUV);
        vec3 grayscale = ((fragColor.xxx + fragColor.yyy) + fragColor.zzz) / vec3(4.0);
        vec3 _206 = mix(grayscale, fragColor.xyz, vec3(vTint.w)) * vTint.xyz;
        fragColor = vec4(_206.x, _206.y, _206.z, fragColor.w);
    }
    else
    {
        vec4 temp = texture(tex, localUV);
        vec3 _218 = fragColor.xyz * temp.w;
        fragColor = vec4(_218.x, _218.y, _218.z, fragColor.w);
        fragColor.w = temp.w;
    }
}

