#version 330

layout(std140) uniform fsConstants
{
    vec2 resolution;
    float secondsSinceGameLaunch;
    uint tick;
    float opacity;
    float guiScale;
} _34;

uniform sampler2D tex;

in vec2 vUV;
layout(location = 0) out vec4 fragColor;

float seconds()
{
    return _34.secondsSinceGameLaunch;
}

float jitterActive()
{
    float sinSample = sin(seconds() * 0.139644443988800048828125);
    float timingActive = step(0.997777760028839111328125, sinSample);
    float fadeInActive = abs(1.0 - step(1.0, _34.opacity));
    return step(1.0, timingActive + fadeInActive);
}

vec2 jitterUvs(inout vec2 uv)
{
    float bigWobble = sin((uv.y * 15.0) + (seconds() * 2.0)) / 100.0;
    float smallJitter = 1.0 + (sin(seconds() * 100.0) / 2.0);
    uv.x += (jitterActive() * (bigWobble * smallJitter));
    return uv;
}

float stripes(vec2 uv)
{
    float width = ceil(2.0 * _34.guiScale);
    float offset = seconds() * 20.0;
    float y = (uv.y * _34.resolution.y) + offset;
    y = floor(y / width);
    return float(uint(y) & 1u);
}

vec4 alphaBlend(vec4 src, vec4 dest)
{
    vec3 _137 = mix(dest.xyz, src.xyz, vec3(src.w));
    vec4 ret;
    ret = vec4(_137.x, _137.y, _137.z, ret.w);
    ret.w = src.w + ((1.0 - src.w) * dest.w);
    return ret;
}

float flickerAlpha()
{
    return 1.0;
}

void main()
{
    vec2 param = vUV;
    vec2 _160 = jitterUvs(param);
    vec2 uv = _160;
    vec4 texSample = texture(tex, uv);
    vec2 param_1 = uv;
    vec4 stripesColor = vec4(1.0, 1.0, 1.0, stripes(param_1) * 0.3499999940395355224609375);
    vec4 param_2 = stripesColor;
    vec4 param_3 = texSample;
    vec4 texWithStripes = alphaBlend(param_2, param_3);
    float pick = 1.0 - step(1.0, 1.0 - texSample.w);
    fragColor = mix(texSample, texWithStripes, vec4(pick));
    fragColor.w *= flickerAlpha();
    fragColor.w *= _34.opacity;
}

