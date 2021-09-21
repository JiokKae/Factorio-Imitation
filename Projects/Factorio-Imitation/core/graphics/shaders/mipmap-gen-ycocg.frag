#version 330

layout(std140) uniform mipMapGenParams
{
    ivec2 offset;
    int unusedLevel;
} _85;

uniform sampler2D tex;
uniform sampler2D tex2;

layout(location = 0) out vec4 fragColor;
in vec2 vUV;

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
    vec4 color = vec4(0.0);
    ivec2 coord = (ivec2(2) * ivec2(gl_FragCoord.xy)) + _85.offset;
    for (int y = 0; y < 2; y++)
    {
        for (int x = 0; x < 2; x++)
        {
            vec4 t1 = texelFetch(tex, coord + ivec2(x, y), 0);
            float a1 = texelFetch(tex2, coord + ivec2(x, y), 0).x;
            vec4 param = t1;
            float param_1 = a1;
            t1 = YCoCgToRGB(param, param_1);
            color += t1;
        }
    }
    fragColor = color * 0.25;
}

