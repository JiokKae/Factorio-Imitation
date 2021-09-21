#version 330

layout(std140) uniform blurConstants
{
    mat4 projection;
    mat4 model;
    vec2 regionStart;
    vec2 regionSize;
    vec4 coeff0;
    vec4 coeff1234;
    vec2 direction;
} _49;

uniform sampler2D tex;

layout(location = 0) out vec4 fragColor;
in vec2 vUV;

vec4 blur9(sampler2D image, vec2 uv, vec2 direction)
{
    vec4 color = vec4(0.0);
    vec2 off1 = vec2(1.0) * direction;
    vec2 off2 = vec2(2.0) * direction;
    vec2 off3 = vec2(3.0) * direction;
    vec2 off4 = vec2(4.0) * direction;
    color += (texture(image, uv) * _49.coeff0);
    color += (texture(image, uv + off1) * _49.coeff1234.x);
    color += (texture(image, uv - off1) * _49.coeff1234.x);
    color += (texture(image, uv + off2) * _49.coeff1234.y);
    color += (texture(image, uv - off2) * _49.coeff1234.y);
    color += (texture(image, uv + off3) * _49.coeff1234.z);
    color += (texture(image, uv - off3) * _49.coeff1234.z);
    color += (texture(image, uv + off4) * _49.coeff1234.w);
    color += (texture(image, uv - off4) * _49.coeff1234.w);
    return color;
}

void main()
{
    vec2 param = vUV;
    vec2 param_1 = _49.direction;
    fragColor = blur9(tex, param, param_1);
}

