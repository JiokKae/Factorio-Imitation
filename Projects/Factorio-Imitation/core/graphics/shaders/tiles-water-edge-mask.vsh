cbuffer _42
{
    row_major float4x4 _42_projection : packoffset(c0);
};

static float4 gl_Position;
static uint vFlags;
static uint flags;
static float vVariation;
static float2 vUV;
static float2 uv;
static float2 position;
static uint2 masks;

struct SPIRV_Cross_Input
{
    float2 position : TEXCOORD0;
    float2 uv : TEXCOORD1;
    uint2 masks : TEXCOORD2;
    uint flags : TEXCOORD3;
};

struct SPIRV_Cross_Output
{
    float2 vUV : TEXCOORD0;
    float vVariation : TEXCOORD1;
    nointerpolation uint vFlags : TEXCOORD2;
    float4 gl_Position : SV_Position;
};

void vert_main()
{
    vFlags = flags;
    vVariation = (0.875f + (float(flags >> uint(16)) * 0.0078125f)) * 100.0f;
    vUV = uv;
    gl_Position = mul(float4(position, 0.0f, 1.0f), _42_projection);
}

SPIRV_Cross_Output main(SPIRV_Cross_Input stage_input)
{
    flags = stage_input.flags;
    uv = stage_input.uv;
    position = stage_input.position;
    masks = stage_input.masks;
    vert_main();
    SPIRV_Cross_Output stage_output;
    stage_output.gl_Position = gl_Position;
    stage_output.vFlags = vFlags;
    stage_output.vVariation = vVariation;
    stage_output.vUV = vUV;
    return stage_output;
}
