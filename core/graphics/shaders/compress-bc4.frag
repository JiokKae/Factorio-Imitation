#version 330

layout(std140) uniform mipMapGenParams
{
    ivec2 offset;
    int unusedLevel;
    uint useChannelVec;
    vec4 channelVec;
} _43;

uniform sampler2D tex;

layout(location = 0) out uvec2 fragColor;
in vec2 vUV;
float blockAlpha[16];

void fetchBlock()
{
    ivec2 baseCoord = (ivec2(4) * ivec2(gl_FragCoord.xy)) + _43.offset;
    for (int by = 0; by < 4; by++)
    {
        for (int bx = 0; bx < 4; bx++)
        {
            vec4 color = vec4(0.0);
            ivec2 coord = baseCoord + ivec2(bx, by);
            vec4 t1 = texelFetch(tex, coord, 0);
            float alpha = t1.x;
            if (_43.useChannelVec != 0u)
            {
                alpha = dot(t1, _43.channelVec);
            }
            blockAlpha[(by * 4) + bx] = alpha;
        }
    }
}

void InsetYBBox(inout float mincol, inout float maxcol)
{
    float inset = ((maxcol - mincol) / 32.0) - 0.00196078442968428134918212890625;
    mincol = clamp(mincol + inset, 0.0, 1.0);
    maxcol = clamp(maxcol - inset, 0.0, 1.0);
}

uint EmitAlphaEndPointsYCoCgDXT5(inout float mincol, inout float maxcol)
{
    float param = mincol;
    float param_1 = maxcol;
    InsetYBBox(param, param_1);
    mincol = param;
    maxcol = param_1;
    uint c0 = uint(round(mincol * 255.0));
    uint c1 = uint(round(maxcol * 255.0));
    return (c0 << uint(8)) | c1;
}

uvec2 EmitAlphaIndicesYCoCgDXT5(float minAlpha, float maxAlpha)
{
    float mid = (maxAlpha - minAlpha) / 14.0;
    float ab1 = minAlpha + mid;
    float ab2 = (((6.0 * maxAlpha) + (1.0 * minAlpha)) * 0.14285714924335479736328125) + mid;
    float ab3 = (((5.0 * maxAlpha) + (2.0 * minAlpha)) * 0.14285714924335479736328125) + mid;
    float ab4 = (((4.0 * maxAlpha) + (3.0 * minAlpha)) * 0.14285714924335479736328125) + mid;
    float ab5 = (((3.0 * maxAlpha) + (4.0 * minAlpha)) * 0.14285714924335479736328125) + mid;
    float ab6 = (((2.0 * maxAlpha) + (5.0 * minAlpha)) * 0.14285714924335479736328125) + mid;
    float ab7 = (((1.0 * maxAlpha) + (6.0 * minAlpha)) * 0.14285714924335479736328125) + mid;
    uvec2 indices = uvec2(0u);
    uint index = 0u;
    for (int i = 0; i < 6; i++)
    {
        float a = blockAlpha[i];
        index = 1u;
        index += uint(a <= ab1);
        index += uint(a <= ab2);
        index += uint(a <= ab3);
        index += uint(a <= ab4);
        index += uint(a <= ab5);
        index += uint(a <= ab6);
        index += uint(a <= ab7);
        index &= 7u;
        index ^= uint(2u > index);
        indices.x |= (index << uint((3 * i) + 16));
    }
    indices.y = index >> uint(1);
    for (int i_1 = 6; i_1 < 16; i_1++)
    {
        float a_1 = blockAlpha[i_1];
        index = 1u;
        index += uint(a_1 <= ab1);
        index += uint(a_1 <= ab2);
        index += uint(a_1 <= ab3);
        index += uint(a_1 <= ab4);
        index += uint(a_1 <= ab5);
        index += uint(a_1 <= ab6);
        index += uint(a_1 <= ab7);
        index &= 7u;
        index ^= uint(2u > index);
        indices.y |= (index << uint((3 * i_1) - 16));
    }
    return indices;
}

void main()
{
    fetchBlock();
    float minAlpha = blockAlpha[0];
    float maxAlpha = blockAlpha[0];
    for (int i = 1; i < 16; i++)
    {
        minAlpha = min(minAlpha, blockAlpha[i]);
        maxAlpha = max(maxAlpha, blockAlpha[i]);
    }
    float param = minAlpha;
    float param_1 = maxAlpha;
    uint _423 = EmitAlphaEndPointsYCoCgDXT5(param, param_1);
    minAlpha = param;
    maxAlpha = param_1;
    uvec2 result;
    result.x = _423;
    float param_2 = minAlpha;
    float param_3 = maxAlpha;
    uvec2 indices = EmitAlphaIndicesYCoCgDXT5(param_2, param_3);
    result.x |= indices.x;
    result.y = indices.y;
    fragColor = result;
}

