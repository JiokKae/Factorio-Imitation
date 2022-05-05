#version 330

uniform sampler2D atlasTexture;

in vec2 vUV;
layout(location = 0) out vec4 fragColor;
flat in vec4 vColor;

void main()
{
    vec4 texColor = texture(atlasTexture, vUV);
    fragColor = vColor * texColor.x;
}

