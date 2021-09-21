#version 330

uniform sampler2D tex;

in vec2 vUV;
layout(location = 0) out vec4 fragColor;

void main()
{
    float shadow = texture(tex, vUV).w * 0.5;
    fragColor = vec4(0.0, 0.0, 0.0, shadow);
}

