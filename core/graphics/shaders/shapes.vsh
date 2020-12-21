cbuffer _19
{
    row_major float4x4 _19_projection : packoffset(c0);
};

static float4 gl_Position;
static float2 position;
static float4 vColor;
static float4 color;

struct SPIRV_Cross_Input
{
    float2 position : TEXCOORD0;
    float4 color : TEXCOORD1;
};

struct SPIRV_Cross_Output
{
    float4 vColor : TEXCOORD0;
    float4 gl_Position : SV_Position;
};

void vert_main()
{
    gl_Position = mul(float4(position, 0.0f, 1.0f), _19_projection);
    vColor = color;
}

SPIRV_Cross_Output main(SPIRV_Cross_Input stage_input)
{
    position = stage_input.position;
    color = stage_input.color;
    vert_main();
    SPIRV_Cross_Output stage_output;
    stage_output.gl_Position = gl_Position;
    stage_output.vColor = vColor;
    return stage_output;
}
