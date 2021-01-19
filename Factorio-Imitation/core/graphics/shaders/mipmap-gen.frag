#version 330

layout(std140) uniform mipMapGenParams
{
    ivec2 offset;
    int unusedLevel;
} _27;

uniform sampler2D tex;

layout(location = 0) out vec4 fragColor;
in vec2 vUV;

void main()
{
    vec4 color = vec4(0.0);
    ivec2 coord = (ivec2(2) * ivec2(gl_FragCoord.xy)) + _27.offset;
    for (int y = 0; y < 2; y++)
    {
        for (int x = 0; x < 2; x++)
        {
            vec4 t1 = texelFetch(tex, coord + ivec2(x, y), 0);
            color += t1;
        }
    }
    fragColor = color * 0.25;
}

