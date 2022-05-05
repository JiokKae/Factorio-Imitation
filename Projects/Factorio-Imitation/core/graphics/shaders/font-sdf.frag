#version 330

layout(std140) uniform fsConstants
{
    float borderWidthInPixels;
    float onEdgeValue;
    float pixelDistScale;
} _31;

uniform sampler2D atlasTexture;

flat in vec4 vColor;
in vec2 vUV;
flat in vec4 vBorderColor;
layout(location = 0) out vec4 fragColor;

float getIntensity(float borderWidth, float smoothing, vec2 uv)
{
    float sdfValue = texture(atlasTexture, uv).x;
    float intensity = smoothstep((_31.onEdgeValue - borderWidth) - smoothing, (_31.onEdgeValue - borderWidth) + smoothing, sdfValue);
    return intensity;
}

void main()
{
    float borderWidth = _31.borderWidthInPixels * _31.pixelDistScale;
    float smoothing = _31.pixelDistScale * 0.5;
    vec4 useColor = vColor;
    float param = borderWidth;
    float param_1 = smoothing;
    vec2 param_2 = vUV;
    float intensity = getIntensity(param, param_1, param_2);
    float dscale = 0.100000001490116119384765625;
    vec2 duv = (dFdx(vUV) + dFdy(vUV)) * dscale;
    float param_3 = borderWidth;
    float param_4 = smoothing;
    vec2 param_5 = vUV + vec2(duv.x, duv.y);
    float a = getIntensity(param_3, param_4, param_5);
    float param_6 = borderWidth;
    float param_7 = smoothing;
    vec2 param_8 = vUV + vec2(duv.x, -duv.y);
    float b = getIntensity(param_6, param_7, param_8);
    float param_9 = borderWidth;
    float param_10 = smoothing;
    vec2 param_11 = vUV + vec2(-duv.x, duv.y);
    float c = getIntensity(param_9, param_10, param_11);
    float param_12 = borderWidth;
    float param_13 = smoothing;
    vec2 param_14 = vUV + vec2(-duv.x, -duv.y);
    float d = getIntensity(param_12, param_13, param_14);
    float avg = (((a + b) + c) + d) / 4.0;
    intensity = mix(intensity, avg, 0.5);
    bvec4 _173 = bvec4(borderWidth > 0.0);
    vec4 useBorderColor = vec4(_173.x ? vBorderColor.x : useColor.x, _173.y ? vBorderColor.y : useColor.y, _173.z ? vBorderColor.z : useColor.z, _173.w ? vBorderColor.w : useColor.w);
    float sdfValue = texture(atlasTexture, vUV).x;
    float borderIntensity = smoothstep(_31.onEdgeValue - smoothing, _31.onEdgeValue + smoothing, sdfValue);
    useColor = mix(useBorderColor, useColor, vec4(borderIntensity));
    fragColor = useColor * intensity;
}

