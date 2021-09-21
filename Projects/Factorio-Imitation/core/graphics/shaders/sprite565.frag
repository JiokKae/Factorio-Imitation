#version 330

uniform usampler2D tex;

in vec2 vUV;
layout(location = 0) out vec4 fragColor;
in vec4 vTintWith565Multiplier;

vec3 unpackRGB565(int rgb5)
{
    return vec3(ivec3(rgb5) & ivec3(63488, 2016, 31));
}

void main()
{
    ivec2 size = textureSize(tex, 0);
    vec2 coord = floor(vUV * vec2(size));
    int rgb5 = int(texelFetch(tex, ivec2(coord), 0).x);
    int param = rgb5;
    vec3 _67 = unpackRGB565(param) * vTintWith565Multiplier.xyz;
    fragColor = vec4(_67.x, _67.y, _67.z, fragColor.w);
    fragColor.w = vTintWith565Multiplier.w;
}

