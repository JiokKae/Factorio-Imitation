cbuffer _20
{
    row_major float4x4 _20_projection : packoffset(c0);
    row_major float4x4 _20_model : packoffset(c4);
    float2 _20_regionStart : packoffset(c8);
    float2 _20_regionSize : packoffset(c8.z);
    float4 _20_coeff0 : packoffset(c9);
    float4 _20_coeff1234 : packoffset(c10);
    float2 _20_direction : packoffset(c11);
};

static float4 gl_Position;
static float2 position;
static float2 vUV;
static float2 uv;

struct SPIRV_Cross_Input
{
    float2 position : TEXCOORD0;
    float2 uv : TEXCOORD1;
};

struct SPIRV_Cross_Output
{
    float2 vUV : TEXCOORD0;
    float4 gl_Position : SV_Position;
};

void vert_main()
{
    gl_Position = mul(float4(position, 0.0f, 1.0f), mul(_20_model, _20_projection));
    vUV = _20_regionStart + (uv * _20_regionSize);
}

SPIRV_Cross_Output main(SPIRV_Cross_Input stage_input)
{
    position = stage_input.position;
    uv = stage_input.uv;
    vert_main();
    SPIRV_Cross_Output stage_output;
    stage_output.gl_Position = gl_Position;
    stage_output.vUV = vUV;
    return stage_output;
}
