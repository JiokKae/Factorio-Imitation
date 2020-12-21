cbuffer _30
{
    row_major float4x4 _30_projection : packoffset(c0);
};

static float4 gl_Position;
static float4 vColor;
static float4 color;
static float2 vUV;
static float2 uv;
static float2 position;
static uint2 masks;

struct SPIRV_Cross_Input
{
    float2 position : TEXCOORD0;
    float2 uv : TEXCOORD1;
    uint2 masks : TEXCOORD2;
    float4 color : TEXCOORD3;
};

struct SPIRV_Cross_Output
{
    float2 vUV : TEXCOORD0;
    float4 vColor : TEXCOORD1;
    float4 gl_Position : SV_Position;
};

void vert_main()
{
    vColor = color;
    vUV = uv;
    gl_Position = mul(float4(position, 0.0f, 1.0f), _30_projection);
}

SPIRV_Cross_Output main(SPIRV_Cross_Input stage_input)
{
    color = stage_input.color;
    uv = stage_input.uv;
    position = stage_input.position;
    masks = stage_input.masks;
    vert_main();
    SPIRV_Cross_Output stage_output;
    stage_output.gl_Position = gl_Position;
    stage_output.vColor = vColor;
    stage_output.vUV = vUV;
    return stage_output;
}
