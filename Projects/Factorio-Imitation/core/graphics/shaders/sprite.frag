#version 330

uniform sampler2D tex;

in vec2 vUV;
in vec4 vTint;
flat in uint vExtra;
layout(location = 0) out vec4 fragColor;

void main()
{
    vec4 color = texture(tex, vUV);
    color *= vTint;
    if (vExtra != 0u)
    {
        vec3 _42 = vec3(dot(color.xyz, vec3(0.2989999949932098388671875, 0.58700001239776611328125, 0.114000000059604644775390625)));
        color = vec4(_42.x, _42.y, _42.z, color.w);
    }
    fragColor = color;
}

