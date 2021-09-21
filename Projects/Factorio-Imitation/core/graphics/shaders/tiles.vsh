cbuffer _146
{
    row_major float4x4 _146_projection : packoffset(c0);
};
Buffer<float4> maskTexCoordTable : register(t0);

static float4 gl_Position;
static int gl_VertexIndex;
static uint vFlags;
static uint flags;
static uint2 masks;
static float4 maskUVs;
static float3 vTint;
static float2 vUV;
static float2 uv;
static float2 position;

struct SPIRV_Cross_Input
{
    float2 position : TEXCOORD0;
    float2 uv : TEXCOORD1;
    uint2 masks : TEXCOORD2;
    uint flags : TEXCOORD3;
    uint gl_VertexIndex : SV_VertexID;
};

struct SPIRV_Cross_Output
{
    float2 vUV : TEXCOORD0;
    float4 maskUVs : TEXCOORD1;
    nointerpolation uint vFlags : TEXCOORD2;
    float3 vTint : TEXCOORD3;
    float4 gl_Position : SV_Position;
};

float2 getMaskUV(float2 corner, uint maskIndex)
{
    float4 mask = maskTexCoordTable.Load(int(maskIndex));
    return mask.xy + (corner * mask.zw);
}

float3 unpackRGB565(int rgb5)
{
    return float3(rgb5.xxx & int3(63488, 2016, 31));
}

float3 decodeRGB565(int rgb5)
{
    int param = rgb5;
    return float3(unpackRGB565(param) * float3(1.5751007595099508762359619140625e-05f, 0.0004960317746736109256744384765625f, 0.0322580635547637939453125f));
}

void vert_main()
{
    vFlags = flags;
    uint vertexID = uint(gl_VertexIndex) % 4u;
    uint i = vertexID / 2u;
    uint j = vertexID % 2u;
    float2 corner = float2(float(i), float(j));
    float2 param = corner;
    uint param_1 = masks.x;
    float2 mask1 = getMaskUV(param, param_1);
    float2 param_2 = corner;
    uint param_3 = masks.y;
    float2 mask2 = getMaskUV(param_2, param_3);
    maskUVs = float4(mask1, mask2);
    int param_4 = int(flags >> uint(16));
    vTint = decodeRGB565(param_4);
    vUV = uv;
    gl_Position = mul(float4(position, 0.0f, 1.0f), _146_projection);
}

SPIRV_Cross_Output main(SPIRV_Cross_Input stage_input)
{
    gl_VertexIndex = int(stage_input.gl_VertexIndex);
    flags = stage_input.flags;
    masks = stage_input.masks;
    uv = stage_input.uv;
    position = stage_input.position;
    vert_main();
    SPIRV_Cross_Output stage_output;
    stage_output.gl_Position = gl_Position;
    stage_output.vFlags = vFlags;
    stage_output.maskUVs = maskUVs;
    stage_output.vTint = vTint;
    stage_output.vUV = vUV;
    return stage_output;
}
