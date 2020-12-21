#version 330

uniform sampler2D tex;

layout(location = 0) out vec4 fragColor;
in vec2 vUV;
in vec4 vTint;

void main()
{
    fragColor = vTint * texture(tex, vUV).x;
}

