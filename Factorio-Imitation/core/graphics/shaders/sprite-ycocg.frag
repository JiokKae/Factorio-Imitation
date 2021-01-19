#version 330

uniform sampler2D tex1;
uniform sampler2D tex2;

in vec2 vUV;
in vec4 vTint;
flat in uint vExtra;
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
    vec4 yCoCg = texture(tex1, vUV);
    float alpha = texture(tex2, vUV).x;
    vec4 param = yCoCg;
    float param_1 = alpha;
    vec4 color = YCoCgToRGB(param, param_1);
    color *= vTint;
    if (vExtra != 0u)
    {
        vec3 _110 = vec3(dot(color.xyz, vec3(0.2989999949932098388671875, 0.58700001239776611328125, 0.114000000059604644775390625)));
        color = vec4(_110.x, _110.y, _110.z, color.w);
    }
    fragColor = color;
}

