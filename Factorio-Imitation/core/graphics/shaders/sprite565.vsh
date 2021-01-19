cbuffer _19
{
    row_major float4x4 _19_projection : packoffset(c0);
};

static float4 gl_Position;
static float3 position;
static float2 vUV;
static float2 uv;
static float4 vTintWith565Multiplier;
static float4 tint;

struct SPIRV_Cross_Input
{
    float3 position : TEXCOORD0;
    float2 uv : TEXCOORD1;
    float4 tint : TEXCOORD2;
};

struct SPIRV_Cross_Output
{
    float2 vUV : TEXCOORD0;
    float4 vTintWith565Multiplier : TEXCOORD1;
    float4 gl_Position : SV_Position;
};

void vert_main()
{
    gl_Position = mul(float4(position, 1.0f), _19_projection);
    vUV = uv;
    vTintWith565Multiplier = float4(tint.xyz * float3(1.5751007595099508762359619140625e-05f, 0.0004960317746736109256744384765625f, 0.0322580635547637939453125f), tint.w);
}

SPIRV_Cross_Output main(SPIRV_Cross_Input stage_input)
{
    position = stage_input.position;
    uv = stage_input.uv;
    tint = stage_input.tint;
    vert_main();
    SPIRV_Cross_Output stage_output;
    stage_output.gl_Position = gl_Position;
    stage_output.vUV = vUV;
    stage_output.vTintWith565Multiplier = vTintWith565Multiplier;
    return stage_output;
}
