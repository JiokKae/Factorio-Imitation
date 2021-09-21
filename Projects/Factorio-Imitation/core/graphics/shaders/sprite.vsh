cbuffer _19
{
    row_major float4x4 _19_projection : packoffset(c0);
};

static float4 gl_Position;
static float3 position;
static float2 vUV;
static float2 uv;
static float4 vTint;
static float4 tint;
static uint vExtra;
static uint extra;

struct SPIRV_Cross_Input
{
    float3 position : TEXCOORD0;
    float2 uv : TEXCOORD1;
    float4 tint : TEXCOORD2;
    uint extra : TEXCOORD3;
};

struct SPIRV_Cross_Output
{
    float2 vUV : TEXCOORD0;
    float4 vTint : TEXCOORD1;
    nointerpolation uint vExtra : TEXCOORD2;
    float4 gl_Position : SV_Position;
};

void vert_main()
{
    gl_Position = mul(float4(position, 1.0f), _19_projection);
    vUV = uv;
    vTint = tint;
    vExtra = extra;
}

SPIRV_Cross_Output main(SPIRV_Cross_Input stage_input)
{
    position = stage_input.position;
    uv = stage_input.uv;
    tint = stage_input.tint;
    extra = stage_input.extra;
    vert_main();
    SPIRV_Cross_Output stage_output;
    stage_output.gl_Position = gl_Position;
    stage_output.vUV = vUV;
    stage_output.vTint = vTint;
    stage_output.vExtra = vExtra;
    return stage_output;
}
