#version 330

layout(std140) uniform mipMapGenParams
{
    ivec2 texOffset;
    int unusedLevel;
    uint unusedFromRGBA;
} _136;

uniform sampler2D tex;
uniform sampler2D tex2;

layout(location = 0) out uvec4 fragColor;
in vec2 vUV;
vec3 block[16];

vec3 toYCoCg(vec3 c)
{
    float Y = ((c.x + (2.0 * c.y)) + c.z) * 0.25;
    float Co = (((2.0 * c.x) - (2.0 * c.z)) * 0.25) + 0.501960813999176025390625;
    float Cg = ((((-c.x) + (2.0 * c.y)) - c.z) * 0.25) + 0.501960813999176025390625;
    return vec3(Y, Co, Cg);
}

void fetchBlock()
{
    ivec2 baseCoord = (ivec2(4) * ivec2(gl_FragCoord.xy)) + _136.texOffset;
    for (int by = 0; by < 4; by++)
    {
        for (int bx = 0; bx < 4; bx++)
        {
            ivec2 coord = baseCoord + ivec2(bx, by);
            vec3 color = texelFetch(tex, coord, 0).xyz;
            vec3 param = color;
            block[(by * 4) + bx] = toYCoCg(param);
        }
    }
}

void FindMinMaxColorsBox(inout vec3 mincol, inout vec3 maxcol)
{
    mincol = vec3(1.0);
    maxcol = vec3(0.0);
    for (int i = 0; i < 16; i++)
    {
        mincol = min(mincol, block[i]);
        maxcol = max(maxcol, block[i]);
    }
}

void SelectYCoCgDiagonal(inout vec2 minColor, inout vec2 maxColor)
{
    vec2 mid = (maxColor + minColor) * 0.5;
    float cov = 0.0;
    for (int i = 0; i < 16; i++)
    {
        vec2 t = block[i].yz - mid;
        cov += (t.x * t.y);
    }
    if (cov < 0.0)
    {
        float tmp = maxColor.y;
        maxColor.y = minColor.y;
        minColor.y = tmp;
    }
}

int GetYCoCgScale(vec2 minColor, vec2 maxColor)
{
    vec2 m0 = abs(minColor - vec2(0.501960813999176025390625));
    vec2 m1 = abs(maxColor - vec2(0.501960813999176025390625));
    float m = max(max(m0.x, m0.y), max(m1.x, m1.y));
    int scale = 1;
    if (m < 0.2509804069995880126953125)
    {
        scale = 2;
    }
    if (m < 0.12549020349979400634765625)
    {
        scale = 4;
    }
    return scale;
}

void InsetCoCgBBox(inout vec2 mincol, inout vec2 maxcol)
{
    vec2 inset = ((maxcol - mincol) / vec2(16.0)) - vec2(0.00196078442968428134918212890625);
    mincol = clamp(mincol + inset, vec2(0.0), vec2(1.0));
    maxcol = clamp(maxcol - inset, vec2(0.0), vec2(1.0));
}

uint EmitEndPointsYCoCgDXT5(inout vec2 mincol, inout vec2 maxcol, int scale)
{
    maxcol = ((maxcol - vec2(0.501960813999176025390625)) * float(scale)) + vec2(0.501960813999176025390625);
    mincol = ((mincol - vec2(0.501960813999176025390625)) * float(scale)) + vec2(0.501960813999176025390625);
    vec2 param = mincol;
    vec2 param_1 = maxcol;
    InsetCoCgBBox(param, param_1);
    mincol = param;
    maxcol = param_1;
    maxcol = round(maxcol * vec2(31.0, 63.0));
    mincol = round(mincol * vec2(31.0, 63.0));
    ivec2 imaxcol = ivec2(maxcol);
    ivec2 imincol = ivec2(mincol);
    uvec2 result;
    result.x = uint(((imaxcol.x << 11) | (imaxcol.y << 5)) | (scale - 1));
    result.y = uint(((imincol.x << 11) | (imincol.y << 5)) | (scale - 1));
    imaxcol.x = (imaxcol.x << 3) | (imaxcol.x >> 2);
    imaxcol.y = (imaxcol.y << 2) | (imaxcol.y >> 4);
    imincol.x = (imincol.x << 3) | (imincol.x >> 2);
    imincol.y = (imincol.y << 2) | (imincol.y >> 4);
    maxcol = vec2(imaxcol) * 0.0039215688593685626983642578125;
    mincol = vec2(imincol) * 0.0039215688593685626983642578125;
    maxcol = ((maxcol - vec2(0.501960813999176025390625)) / vec2(float(scale))) + vec2(0.501960813999176025390625);
    mincol = ((mincol - vec2(0.501960813999176025390625)) / vec2(float(scale))) + vec2(0.501960813999176025390625);
    return result.x | (result.y << uint(16));
}

float colorDistance(vec2 c0, vec2 c1)
{
    return dot(c0 - c1, c0 - c1);
}

uint EmitIndicesYCoCgDXT5(vec2 mincol, vec2 maxcol)
{
    vec2 c[4];
    c[0] = maxcol;
    c[1] = mincol;
    c[2] = mix(c[0], c[1], vec2(0.3333333432674407958984375));
    c[3] = mix(c[0], c[1], vec2(0.666666686534881591796875));
    uint indices = 0u;
    for (int i = 0; i < 16; i++)
    {
        vec2 param = block[i].yz;
        vec2 param_1 = c[0];
        vec4 dist;
        dist.x = colorDistance(param, param_1);
        vec2 param_2 = block[i].yz;
        vec2 param_3 = c[1];
        dist.y = colorDistance(param_2, param_3);
        vec2 param_4 = block[i].yz;
        vec2 param_5 = c[2];
        dist.z = colorDistance(param_4, param_5);
        vec2 param_6 = block[i].yz;
        vec2 param_7 = c[3];
        dist.w = colorDistance(param_6, param_7);
        bvec4 _545 = greaterThan(dist.xyxy, dist.wzzw);
        uvec4 b = uvec4(_545.x ? uvec4(1u).x : uvec4(0u).x, _545.y ? uvec4(1u).y : uvec4(0u).y, _545.z ? uvec4(1u).z : uvec4(0u).z, _545.w ? uvec4(1u).w : uvec4(0u).w);
        uint b4 = uint(dist.z > dist.w);
        uint index = (b.x & b4) | (((b.y & b.z) | (b.x & b.w)) << uint(1));
        indices |= (index << uint(i * 2));
    }
    return indices;
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
        float a = block[i].x;
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
        float a_1 = block[i_1].x;
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

uvec4 compress_YCoCg_DXT_fp()
{
    vec3 param;
    vec3 param_1;
    FindMinMaxColorsBox(param, param_1);
    vec3 mincol = param;
    vec3 maxcol = param_1;
    vec2 param_2 = mincol.yz;
    vec2 param_3 = maxcol.yz;
    SelectYCoCgDiagonal(param_2, param_3);
    mincol = vec3(mincol.x, param_2.x, param_2.y);
    maxcol = vec3(maxcol.x, param_3.x, param_3.y);
    vec2 param_4 = mincol.yz;
    vec2 param_5 = maxcol.yz;
    int scale = GetYCoCgScale(param_4, param_5);
    vec2 param_6 = mincol.yz;
    vec2 param_7 = maxcol.yz;
    int param_8 = scale;
    uint _887 = EmitEndPointsYCoCgDXT5(param_6, param_7, param_8);
    mincol = vec3(mincol.x, param_6.x, param_6.y);
    maxcol = vec3(maxcol.x, param_7.x, param_7.y);
    uvec4 result;
    result.z = _887;
    vec2 param_9 = mincol.yz;
    vec2 param_10 = maxcol.yz;
    result.w = EmitIndicesYCoCgDXT5(param_9, param_10);
    float param_11 = mincol.x;
    float param_12 = maxcol.x;
    uint _909 = EmitAlphaEndPointsYCoCgDXT5(param_11, param_12);
    mincol.x = param_11;
    maxcol.x = param_12;
    result.x = _909;
    float param_13 = mincol.x;
    float param_14 = maxcol.x;
    uvec2 indices = EmitAlphaIndicesYCoCgDXT5(param_13, param_14);
    result.x |= indices.x;
    result.y = indices.y;
    return result;
}

void main()
{
    fetchBlock();
    fragColor = compress_YCoCg_DXT_fp();
}

