#version 330

uniform sampler2D mask1Texture;
uniform sampler2D mask2Texture;
uniform sampler2D atlasTexture;

in vec4 maskUVs;
in vec2 vUV;
layout(location = 0) out vec4 fragColor;
in vec3 vTint;
flat in uint vFlags;

void main()
{
    vec2 mask1UV = maskUVs.xy;
    float mask1 = 1.0;
    if (mask1UV.x > (-1.0))
    {
        mask1 = texture(mask1Texture, mask1UV).x;
    }
    float mask2 = texelFetch(mask2Texture, ivec2(gl_FragCoord.xy), 0).y;
    vec4 texel = texture(atlasTexture, vUV);
    mask2 = smoothstep(0.0, 0.5, mask2);
    fragColor = texel * ((1.0 - mask2) * mask1);
    vec3 _75 = fragColor.xyz * vTint;
    fragColor = vec4(_75.x, _75.y, _75.z, fragColor.w);
}

