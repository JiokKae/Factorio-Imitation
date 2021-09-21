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
    float mask = 1.0;
    vec2 mask1UV = maskUVs.xy;
    vec2 mask2UV = maskUVs.zw;
    if (mask1UV.x > (-1.0))
    {
        mask = texture(mask1Texture, mask1UV).x;
        if (mask2UV.x > (-1.0))
        {
            mask *= texture(mask2Texture, mask2UV).x;
        }
    }
    vec4 texel = texture(atlasTexture, vUV);
    fragColor = texel * mask;
    vec3 _69 = fragColor.xyz * vTint;
    fragColor = vec4(_69.x, _69.y, _69.z, fragColor.w);
}

