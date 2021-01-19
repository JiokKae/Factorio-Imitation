#version 330

layout(std140) uniform turretRangeData
{
    uint dataSize;
    vec4 color;
} _47;

uniform sampler2D pretest;

in vec2 vUV;
layout(location = 0) out vec4 fragColor;

void main()
{
    float a = texture(pretest, vUV).w;
    if (a == 0.0)
    {
        fragColor = vec4(0.0);
        return;
    }
    if (a == 1.0)
    {
        vec4 _40;
        if (false)
        {
            _40 = vec4(1.0, 1.0, 0.0, 1.0);
        }
        else
        {
            _40 = _47.color;
        }
        fragColor = _40;
        return;
    }
    discard;
}

