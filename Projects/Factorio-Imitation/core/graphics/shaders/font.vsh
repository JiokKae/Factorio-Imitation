cbuffer _19
{
    row_major float4x4 _19_projection : packoffset(c0);
};

static float4 gl_Position;
static float2 position;
static float2 vUV;
static float2 uv;
static float4 vColor;
static float4 color;
static float4 vBorderColor;
static float4 borderColor;

struct SPIRV_Cross_Input
{
    float2 position : TEXCOORD0;
    float2 uv : TEXCOORD1;
    float4 color : TEXCOORD2;
    float4 borderColor : TEXCOORD3;
};

struct SPIRV_Cross_Output
{
    float2 vUV : TEXCOORD0;
    nointerpolation float4 vColor : TEXCOORD1;
    nointerpolation float4 vBorderColor : TEXCOORD2;
    float4 gl_Position : SV_Position;
};

void vert_main()
{
    gl_Position = mul(float4(position, 0.0f, 1.0f), _19_projection);
    vUV = uv;
    vColor = color;
    vBorderColor = borderColor;
}

SPIRV_Cross_Output main(SPIRV_Cross_Input stage_input)
{
    position = stage_input.position;
    uv = stage_input.uv;
    color = stage_input.color;
    borderColor = stage_input.borderColor;
    vert_main();
    SPIRV_Cross_Output stage_output;
    stage_output.gl_Position = gl_Position;
    stage_output.vUV = vUV;
    stage_output.vColor = vColor;
    stage_output.vBorderColor = vBorderColor;
    return stage_output;
}
