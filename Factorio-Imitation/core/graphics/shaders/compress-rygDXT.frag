#version 330

const uvec4 _387[256] = uvec4[](uvec4(0u), uvec4(0u, 0u, 0u, 1u), uvec4(0u, 1u, 1u, 0u), uvec4(0u, 1u, 1u, 0u), uvec4(1u, 0u, 1u, 1u), uvec4(1u, 0u, 2u, 0u), uvec4(1u, 0u, 2u, 1u), uvec4(1u, 1u, 3u, 0u), uvec4(1u, 1u, 3u, 0u), uvec4(2u, 0u, 3u, 1u), uvec4(2u, 0u, 4u, 0u), uvec4(0u, 4u, 4u, 0u), uvec4(2u, 1u, 4u, 1u), uvec4(2u, 1u, 5u, 0u), uvec4(2u, 1u, 5u, 1u), uvec4(3u, 0u, 6u, 0u), uvec4(3u, 0u, 6u, 0u), uvec4(3u, 0u, 6u, 1u), uvec4(3u, 1u, 7u, 0u), uvec4(1u, 5u, 7u, 0u), uvec4(3u, 2u, 7u, 1u), uvec4(3u, 2u, 8u, 0u), uvec4(4u, 0u, 8u, 1u), uvec4(4u, 0u, 8u, 1u), uvec4(4u, 1u, 8u, 2u), uvec4(4u, 1u, 9u, 1u), uvec4(4u, 2u, 9u, 2u), uvec4(4u, 2u, 9u, 2u), uvec4(4u, 2u, 9u, 3u), uvec4(3u, 5u, 10u, 2u), uvec4(5u, 1u, 10u, 3u), uvec4(5u, 1u, 10u, 3u), uvec4(5u, 2u, 10u, 4u), uvec4(4u, 4u, 11u, 3u), uvec4(5u, 3u, 11u, 4u), uvec4(5u, 3u, 11u, 4u), uvec4(5u, 3u, 11u, 5u), uvec4(6u, 2u, 12u, 4u), uvec4(6u, 2u, 12u, 5u), uvec4(6u, 2u, 12u, 5u), uvec4(6u, 3u, 12u, 6u), uvec4(5u, 5u, 13u, 5u), uvec4(6u, 4u, 13u, 6u), uvec4(6u, 4u, 8u, 16u), uvec4(4u, 8u, 13u, 7u), uvec4(7u, 3u, 14u, 6u), uvec4(7u, 3u, 14u, 7u), uvec4(7u, 3u, 9u, 17u), uvec4(7u, 4u, 14u, 8u), uvec4(7u, 4u, 15u, 7u), uvec4(7u, 4u, 15u, 8u), uvec4(7u, 5u, 11u, 16u), uvec4(5u, 9u, 15u, 9u), uvec4(7u, 6u, 15u, 10u), uvec4(7u, 6u, 16u, 8u), uvec4(8u, 4u, 16u, 9u), uvec4(8u, 4u, 16u, 10u), uvec4(8u, 5u, 15u, 13u), uvec4(8u, 5u, 17u, 9u), uvec4(8u, 6u, 17u, 10u), uvec4(8u, 6u, 17u, 11u), uvec4(8u, 6u, 15u, 16u), uvec4(7u, 9u, 18u, 10u), uvec4(9u, 5u, 18u, 11u), uvec4(9u, 5u, 18u, 12u), uvec4(9u, 6u, 16u, 16u), uvec4(8u, 8u, 19u, 11u), uvec4(9u, 7u, 19u, 12u), uvec4(9u, 7u, 19u, 13u), uvec4(9u, 7u, 17u, 17u), uvec4(10u, 6u, 20u, 12u), uvec4(10u, 6u, 20u, 13u), uvec4(10u, 6u, 20u, 14u), uvec4(10u, 7u, 19u, 16u), uvec4(9u, 9u, 21u, 13u), uvec4(10u, 8u, 21u, 14u), uvec4(10u, 8u, 21u, 15u), uvec4(8u, 12u, 20u, 17u), uvec4(11u, 7u, 22u, 14u), uvec4(11u, 7u, 22u, 15u), uvec4(11u, 7u, 25u, 10u), uvec4(11u, 8u, 22u, 16u), uvec4(11u, 8u, 23u, 15u), uvec4(11u, 8u, 23u, 16u), uvec4(11u, 9u, 26u, 11u), uvec4(9u, 13u, 23u, 17u), uvec4(11u, 10u, 24u, 16u), uvec4(11u, 10u, 24u, 17u), uvec4(12u, 8u, 27u, 12u), uvec4(12u, 8u, 24u, 18u), uvec4(12u, 9u, 25u, 17u), uvec4(12u, 9u, 25u, 18u), uvec4(12u, 10u, 28u, 13u), uvec4(12u, 10u, 25u, 19u), uvec4(12u, 10u, 26u, 18u), uvec4(11u, 13u, 26u, 19u), uvec4(13u, 9u, 29u, 14u), uvec4(13u, 9u, 26u, 20u), uvec4(13u, 10u, 27u, 19u), uvec4(12u, 12u, 27u, 20u), uvec4(13u, 11u, 30u, 15u), uvec4(13u, 11u, 27u, 21u), uvec4(13u, 11u, 28u, 20u), uvec4(14u, 10u, 28u, 21u), uvec4(14u, 10u, 28u, 21u), uvec4(14u, 10u, 28u, 22u), uvec4(14u, 11u, 29u, 21u), uvec4(13u, 13u, 29u, 22u), uvec4(14u, 12u, 24u, 32u), uvec4(14u, 12u, 29u, 23u), uvec4(12u, 16u, 30u, 22u), uvec4(15u, 11u, 30u, 23u), uvec4(15u, 11u, 25u, 33u), uvec4(15u, 11u, 30u, 24u), uvec4(15u, 12u, 31u, 23u), uvec4(15u, 12u, 31u, 24u), uvec4(15u, 12u, 27u, 32u), uvec4(15u, 13u, 31u, 25u), uvec4(13u, 17u, 31u, 26u), uvec4(15u, 14u, 32u, 24u), uvec4(15u, 14u, 32u, 25u), uvec4(16u, 12u, 32u, 26u), uvec4(16u, 12u, 31u, 29u), uvec4(16u, 13u, 33u, 25u), uvec4(16u, 13u, 33u, 26u), uvec4(16u, 14u, 33u, 27u), uvec4(16u, 14u, 31u, 32u), uvec4(16u, 14u, 34u, 26u), uvec4(15u, 17u, 34u, 27u), uvec4(17u, 13u, 34u, 28u), uvec4(17u, 13u, 32u, 32u), uvec4(17u, 14u, 35u, 27u), uvec4(16u, 16u, 35u, 28u), uvec4(17u, 15u, 35u, 29u), uvec4(17u, 15u, 33u, 33u), uvec4(17u, 15u, 36u, 28u), uvec4(18u, 14u, 36u, 29u), uvec4(18u, 14u, 36u, 30u), uvec4(18u, 14u, 35u, 32u), uvec4(18u, 15u, 37u, 29u), uvec4(17u, 17u, 37u, 30u), uvec4(18u, 16u, 37u, 31u), uvec4(18u, 16u, 36u, 33u), uvec4(16u, 20u, 38u, 30u), uvec4(19u, 15u, 38u, 31u), uvec4(19u, 15u, 41u, 26u), uvec4(19u, 15u, 38u, 32u), uvec4(19u, 16u, 39u, 31u), uvec4(19u, 16u, 39u, 32u), uvec4(19u, 16u, 42u, 27u), uvec4(19u, 17u, 39u, 33u), uvec4(17u, 21u, 40u, 32u), uvec4(19u, 18u, 40u, 33u), uvec4(19u, 18u, 43u, 28u), uvec4(20u, 16u, 40u, 34u), uvec4(20u, 16u, 41u, 33u), uvec4(20u, 17u, 41u, 34u), uvec4(20u, 17u, 44u, 29u), uvec4(20u, 18u, 41u, 35u), uvec4(20u, 18u, 42u, 34u), uvec4(20u, 18u, 42u, 35u), uvec4(19u, 21u, 45u, 30u), uvec4(21u, 17u, 42u, 36u), uvec4(21u, 17u, 43u, 35u), uvec4(21u, 18u, 43u, 36u), uvec4(20u, 20u, 46u, 31u), uvec4(21u, 19u, 43u, 37u), uvec4(21u, 19u, 44u, 36u), uvec4(21u, 19u, 44u, 37u), uvec4(22u, 18u, 44u, 37u), uvec4(22u, 18u, 44u, 38u), uvec4(22u, 18u, 45u, 37u), uvec4(22u, 19u, 45u, 38u), uvec4(21u, 21u, 40u, 48u), uvec4(22u, 20u, 45u, 39u), uvec4(22u, 20u, 46u, 38u), uvec4(20u, 24u, 46u, 39u), uvec4(23u, 19u, 41u, 49u), uvec4(23u, 19u, 46u, 40u), uvec4(23u, 19u, 47u, 39u), uvec4(23u, 20u, 47u, 40u), uvec4(23u, 20u, 43u, 48u), uvec4(23u, 20u, 47u, 41u), uvec4(23u, 21u, 47u, 42u), uvec4(21u, 25u, 48u, 40u), uvec4(23u, 22u, 48u, 41u), uvec4(23u, 22u, 48u, 42u), uvec4(24u, 20u, 47u, 45u), uvec4(24u, 20u, 49u, 41u), uvec4(24u, 21u, 49u, 42u), uvec4(24u, 21u, 49u, 43u), uvec4(24u, 22u, 47u, 48u), uvec4(24u, 22u, 50u, 42u), uvec4(24u, 22u, 50u, 43u), uvec4(23u, 25u, 50u, 44u), uvec4(25u, 21u, 48u, 48u), uvec4(25u, 21u, 51u, 43u), uvec4(25u, 22u, 51u, 44u), uvec4(24u, 24u, 51u, 45u), uvec4(25u, 23u, 49u, 49u), uvec4(25u, 23u, 52u, 44u), uvec4(25u, 23u, 52u, 45u), uvec4(26u, 22u, 52u, 46u), uvec4(26u, 22u, 51u, 48u), uvec4(26u, 22u, 53u, 45u), uvec4(26u, 23u, 53u, 46u), uvec4(25u, 25u, 53u, 47u), uvec4(26u, 24u, 52u, 49u), uvec4(26u, 24u, 54u, 46u), uvec4(24u, 28u, 54u, 47u), uvec4(27u, 23u, 57u, 42u), uvec4(27u, 23u, 54u, 48u), uvec4(27u, 23u, 55u, 47u), uvec4(27u, 24u, 55u, 48u), uvec4(27u, 24u, 58u, 43u), uvec4(27u, 24u, 55u, 49u), uvec4(27u, 25u, 56u, 48u), uvec4(25u, 29u, 56u, 49u), uvec4(27u, 26u, 59u, 44u), uvec4(27u, 26u, 56u, 50u), uvec4(28u, 24u, 57u, 49u), uvec4(28u, 24u, 57u, 50u), uvec4(28u, 25u, 60u, 45u), uvec4(28u, 25u, 57u, 51u), uvec4(28u, 26u, 58u, 50u), uvec4(28u, 26u, 58u, 51u), uvec4(28u, 26u, 61u, 46u), uvec4(27u, 29u, 58u, 52u), uvec4(29u, 25u, 59u, 51u), uvec4(29u, 25u, 59u, 52u), uvec4(29u, 26u, 62u, 47u), uvec4(28u, 28u, 59u, 53u), uvec4(29u, 27u, 60u, 52u), uvec4(29u, 27u, 60u, 53u), uvec4(29u, 27u, 60u, 53u), uvec4(30u, 26u, 60u, 54u), uvec4(30u, 26u, 61u, 53u), uvec4(30u, 26u, 61u, 54u), uvec4(30u, 27u, 61u, 54u), uvec4(29u, 29u, 61u, 55u), uvec4(30u, 28u, 62u, 54u), uvec4(30u, 28u, 62u, 55u), uvec4(30u, 28u, 62u, 55u), uvec4(31u, 27u, 62u, 56u), uvec4(31u, 27u, 63u, 55u), uvec4(31u, 27u, 63u, 56u), uvec4(31u, 28u, 63u, 56u), uvec4(31u, 28u, 63u, 57u), uvec4(31u, 28u, 63u, 58u), uvec4(31u, 29u, 63u, 59u), uvec4(31u, 29u, 63u, 59u), uvec4(31u, 30u, 63u, 60u), uvec4(31u, 30u, 63u, 61u), uvec4(31u, 30u, 63u, 62u), uvec4(31u, 31u, 63u, 62u), uvec4(31u, 31u, 63u, 63u));
const float _1384[4] = float[](3.0, 0.0, 2.0, 1.0);
const int _1394[4] = int[](589824, 2304, 262402, 66562);

layout(std140) uniform mipMapGenParams
{
    ivec2 offset;
    int unusedLevel;
    uint noAlphaBlock;
} _419;

uniform sampler2D tex;

layout(location = 0) out uvec4 fragColor;
in vec2 vUV;
vec3 block[16];
float blockAlpha[16];
uvec3 colors[4];

void fetchBlock()
{
    ivec2 baseCoord = (ivec2(4) * ivec2(gl_FragCoord.xy)) + _419.offset;
    for (int by = 0; by < 4; by++)
    {
        for (int bx = 0; bx < 4; bx++)
        {
            ivec2 coord = baseCoord + ivec2(bx, by);
            vec4 color = texelFetch(tex, coord, 0);
            block[(by * 4) + bx] = color.xyz;
            blockAlpha[(by * 4) + bx] = color.w;
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

uvec2 stb_CompressAlphaBlock()
{
    float minAlpha = blockAlpha[0];
    float maxAlpha = blockAlpha[0];
    for (int i = 1; i < 16; i++)
    {
        minAlpha = min(minAlpha, blockAlpha[i]);
        maxAlpha = max(maxAlpha, blockAlpha[i]);
    }
    float param = minAlpha;
    float param_1 = maxAlpha;
    uint _785 = EmitAlphaEndPointsYCoCgDXT5(param, param_1);
    minAlpha = param;
    maxAlpha = param_1;
    uvec2 result;
    result.x = _785;
    float param_2 = minAlpha;
    float param_3 = maxAlpha;
    uvec2 indices = EmitAlphaIndicesYCoCgDXT5(param_2, param_3);
    result.x |= indices.x;
    result.y = indices.y;
    return result;
}

uvec2 getOMatch5(int c)
{
    return _387[c].xy;
}

uvec2 getOMatch6(int c)
{
    return _387[c].zw;
}

uint stb_As16Bit(vec3 c)
{
    uvec3 u = uvec3((c * 255.0) + vec3(0.5));
    u = (u * uvec3(31u, 63u, 31u)) + uvec3(128u);
    u = (u + (u >> uvec3(ivec3(8)))) >> uvec3(ivec3(8));
    return ((u.x << uint(11)) + (u.y << uint(5))) + u.z;
}

uvec2 stb_OptimizeColorsBlock()
{
    vec3 maxv = block[0];
    vec3 minv = block[0];
    vec3 muv = block[0];
    for (int i = 1; i < 16; i++)
    {
        muv += block[i];
        minv = min(minv, block[i]);
        maxv = max(maxv, block[i]);
    }
    muv *= 0.0625;
    float cov[6] = float[](0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    for (int i_1 = 0; i_1 < 16; i_1++)
    {
        vec3 d = block[i_1] - muv;
        cov[0] += (d.x * d.x);
        cov[1] += (d.x * d.y);
        cov[2] += (d.x * d.z);
        cov[3] += (d.y * d.y);
        cov[4] += (d.y * d.z);
        cov[5] += (d.z * d.z);
    }
    for (int i_2 = 0; i_2 < 6; i_2++)
    {
        cov[i_2] *= 255.0;
    }
    vec3 vf = floor(((maxv - minv) * 255.0) + vec3(0.5));
    for (int iter = 0; iter < 4; iter++)
    {
        float r = dot(vf, vec3(cov[0], cov[1], cov[2]));
        float g = dot(vf, vec3(cov[1], cov[3], cov[4]));
        float b = dot(vf, vec3(cov[2], cov[4], cov[5]));
        vf = vec3(r, g, b);
    }
    float magn = max(abs(vf.x), max(abs(vf.y), abs(vf.z)));
    vec3 v_rgb;
    if (magn < 4.0)
    {
        v_rgb = vec3(299.0, 587.0, 114.0);
    }
    else
    {
        v_rgb = vf * (512.0 / magn);
    }
    vec3 minb = block[0];
    vec3 maxb = block[0];
    float _1246 = dot(block[0], v_rgb);
    float mind = _1246;
    float maxd = _1246;
    for (int i_3 = 1; i_3 < 16; i_3++)
    {
        float d_1 = dot(block[i_3], v_rgb);
        if (d_1 < mind)
        {
            mind = d_1;
            minb = block[i_3];
        }
        if (d_1 > maxd)
        {
            maxd = d_1;
            maxb = block[i_3];
        }
    }
    vec3 param = maxb;
    vec3 param_1 = minb;
    return uvec2(stb_As16Bit(param), stb_As16Bit(param_1));
}

uvec3 stb_From16Bit(uint v)
{
    int rv = int((v & 63488u) >> uint(11));
    int gv = int((v & 2016u) >> uint(5));
    int bv = int((v & 31u) >> uint(0));
    uvec3 c = uvec3(uint(rv), uint(gv), uint(bv));
    return (c << uvec3(ivec3(3, 2, 3))) | (c >> uvec3(ivec3(2, 4, 2)));
}

uvec3 stb_Lerp13RGB(uvec3 a, uvec3 b)
{
    return (((uvec3(2u) * a) + b) * uvec3(43691u)) >> uvec3(ivec3(17));
}

void stb_EvalColors(uint c0, uint c1)
{
    uint param = c0;
    colors[0] = stb_From16Bit(param);
    uint param_1 = c1;
    colors[1] = stb_From16Bit(param_1);
    uvec3 param_2 = colors[0];
    uvec3 param_3 = colors[1];
    colors[2] = stb_Lerp13RGB(param_2, param_3);
    uvec3 param_4 = colors[1];
    uvec3 param_5 = colors[0];
    colors[3] = stb_Lerp13RGB(param_4, param_5);
}

uint stb_MatchColorsBlock()
{
    uint mask = 0u;
    vec3 dir = vec3(colors[0]) - vec3(colors[1]);
    float dots[16];
    for (int i = 0; i < 16; i++)
    {
        dots[i] = dot(block[i], dir);
    }
    dir *= 0.0039215688593685626983642578125;
    float stops[4];
    for (int i_1 = 0; i_1 < 4; i_1++)
    {
        stops[i_1] = dot(vec3(colors[i_1]), dir);
    }
    float c0Point = (stops[1] + stops[3]) * 0.5;
    float halfPoint = (stops[3] + stops[2]) * 0.5;
    float c3Point = (stops[2] + stops[0]) * 0.5;
    if (true)
    {
        for (int i_2 = 15; i_2 >= 0; i_2--)
        {
            float d = dots[i_2];
            mask = mask << uint(2);
            if (d < halfPoint)
            {
                mask |= uint((d < c0Point) ? 1 : 3);
            }
            else
            {
                mask |= uint((d < c3Point) ? 2 : 0);
            }
        }
    }
    return mask;
}

bool stb_RefineBlock(inout uint pmax16, inout uint pmin16, uint mask)
{
    uint oldMin = pmin16;
    uint oldMax = pmax16;
    uint max16;
    uint min16;
    if ((mask ^ (mask << uint(2))) < 4u)
    {
        vec3 c = vec3(0.0);
        for (int i = 0; i < 16; i++)
        {
            c += block[i];
        }
        ivec3 rgb = ivec3((c * 15.9375) + vec3(0.5));
        int param = rgb.x;
        int param_1 = rgb.y;
        int param_2 = rgb.z;
        max16 = ((getOMatch5(param).x << uint(11)) | (getOMatch6(param_1).x << uint(5))) | getOMatch5(param_2).x;
        int param_3 = rgb.x;
        int param_4 = rgb.y;
        int param_5 = rgb.z;
        min16 = ((getOMatch5(param_3).y << uint(11)) | (getOMatch6(param_4).y << uint(5))) | getOMatch5(param_5).y;
    }
    else
    {
        vec3 At1 = vec3(0.0);
        vec3 At2 = vec3(0.0);
        int akku = 0;
        uint cm = mask;
        int i_1 = 0;
        for (; i_1 < 16; i_1++, cm = cm >> uint(2))
        {
            uint _step = cm & 3u;
            float w1 = _1384[_step];
            akku += _1394[_step];
            At1 += (block[i_1] * w1);
            At2 += block[i_1];
        }
        At2 = (At2 * 3.0) - At1;
        float xx = float(akku >> 16);
        float yy = float((akku >> 8) & 255);
        float xy = float((akku >> 0) & 255);
        float frb = 93.0 / ((xx * yy) - (xy * xy));
        float fg = frb * 2.03225803375244140625;
        max16 = uint(clamp((((At1.x * yy) - (At2.x * xy)) * frb) + 0.5, 0.0, 31.0)) << uint(11);
        max16 |= (uint(clamp((((At1.y * yy) - (At2.y * xy)) * fg) + 0.5, 0.0, 63.0)) << uint(5));
        max16 |= (uint(clamp((((At1.z * yy) - (At2.z * xy)) * frb) + 0.5, 0.0, 31.0)) << uint(0));
        min16 = uint(clamp((((At2.x * xx) - (At1.x * xy)) * frb) + 0.5, 0.0, 31.0)) << uint(11);
        min16 |= (uint(clamp((((At2.y * xx) - (At1.y * xy)) * fg) + 0.5, 0.0, 63.0)) << uint(5));
        min16 |= (uint(clamp((((At2.z * xx) - (At1.z * xy)) * frb) + 0.5, 0.0, 31.0)) << uint(0));
    }
    pmin16 = min16;
    pmax16 = max16;
    return any(notEqual(uvec2(oldMin, oldMax), uvec2(min16, max16)));
}

uvec2 stb_CompressColorBlock()
{
    bool isConstant = true;
    for (int i = 1; i < 16; i++)
    {
        isConstant = all(bvec4(equal(block[i], block[0]), isConstant));
    }
    uint mask;
    uint max16;
    uint min16;
    if (isConstant)
    {
        ivec3 rgb = ivec3((block[0] * 255.0) + vec3(0.5));
        mask = 2863311530u;
        int param = rgb.x;
        int param_1 = rgb.y;
        int param_2 = rgb.z;
        max16 = ((getOMatch5(param).x << uint(11)) | (getOMatch6(param_1).x << uint(5))) | getOMatch5(param_2).x;
        int param_3 = rgb.x;
        int param_4 = rgb.y;
        int param_5 = rgb.z;
        min16 = ((getOMatch5(param_3).y << uint(11)) | (getOMatch6(param_4).y << uint(5))) | getOMatch5(param_5).y;
    }
    else
    {
        uvec2 maxmin = stb_OptimizeColorsBlock();
        max16 = maxmin.x;
        min16 = maxmin.y;
        if (max16 != min16)
        {
            uint param_6 = max16;
            uint param_7 = min16;
            stb_EvalColors(param_6, param_7);
            mask = stb_MatchColorsBlock();
        }
        else
        {
            mask = 0u;
        }
        uint param_8 = max16;
        uint param_9 = min16;
        uint param_10 = mask;
        bool _1667 = stb_RefineBlock(param_8, param_9, param_10);
        max16 = param_8;
        min16 = param_9;
        if (_1667)
        {
            if (max16 != min16)
            {
                uint param_11 = max16;
                uint param_12 = min16;
                stb_EvalColors(param_11, param_12);
                mask = stb_MatchColorsBlock();
            }
            else
            {
                mask = 0u;
            }
        }
    }
    uvec2 result;
    if (max16 < min16)
    {
        result.x = min16 | (max16 << uint(16));
        result.y = mask ^ 1431655765u;
    }
    else
    {
        result.x = max16 | (min16 << uint(16));
        result.y = mask;
    }
    return result;
}

void main()
{
    fetchBlock();
    uvec2 alphaBlock = stb_CompressAlphaBlock();
    uvec2 _1714 = stb_CompressColorBlock();
    uvec2 colorBlock = _1714;
    uvec4 _1721;
    if (_419.noAlphaBlock != 0u)
    {
        _1721 = uvec4(colorBlock.x, colorBlock.y, 0u, 0u);
    }
    else
    {
        _1721 = uvec4(alphaBlock.x, alphaBlock.y, colorBlock.x, colorBlock.y);
    }
    fragColor = _1721;
}

