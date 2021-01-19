#version 330

uniform sampler2D tex;

layout(location = 0) out vec4 fragColor;
in vec2 vUV;

void main()
{
    fragColor = texture(tex, vUV);
}

