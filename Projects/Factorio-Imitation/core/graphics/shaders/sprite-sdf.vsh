cbuffer _19
{
    row_major float4x4 _19_projection : packoffset(c0);
};

static float4 gl_Position;
static float3 position;
static float2 vUV1;
static float2 uv1;
static float2 vUV2;
static float4 vTint;
static float4 tint;

struct SPIRV_Cross_Input
{
    float3 position : TEXCOORD0;
    float2 uv1 : TEXCOORD1;
    float4 tint : TEXCOORD2;
};

struct SPIRV_Cross_Output
{
    float2 vUV1 : TEXCOORD0;
    float2 vUV2 : TEXCOORD1;
    float4 vTint : TEXCOORD2;
    float4 gl_Position : SV_Position;
};

void vert_main()
{
    gl_Position = mul(float4(position, 1.0f), _19_projection);
    vUV1 = uv1;
    vUV2 = uv1;
    vTint = tint;
}

SPIRV_Cross_Output main(SPIRV_Cross_Input stage_input)
{
    position = stage_input.position;
    uv1 = stage_input.uv1;
    tint = stage_input.tint;
    vert_main();
    SPIRV_Cross_Output stage_output;
    stage_output.gl_Position = gl_Position;
    stage_output.vUV1 = vUV1;
    stage_output.vUV2 = vUV2;
    stage_output.vTint = vTint;
    return stage_output;
}
