#version 330

layout(std140) uniform fsConstants
{
    vec4 tint;
    float lutAlpha;
} _63;

uniform sampler3D lut1;
uniform sampler3D lut2;

layout(location = 0) out vec4 fragColor;
in vec2 vUV;

void main()
{
    ivec3 lu = ivec3(ivec2(gl_FragCoord.xy), 0);
    lu.z = lu.x >> 4;
    lu.x &= 15;
    lu.y = lu.y;
    vec4 lut1Color = texelFetch(lut1, lu, 0);
    vec4 lut2Color = texelFetch(lut2, lu, 0);
    fragColor = clamp(mix(lut1Color, lut2Color, vec4(_63.lutAlpha)) * _63.tint, vec4(0.0), vec4(1.0));
}

