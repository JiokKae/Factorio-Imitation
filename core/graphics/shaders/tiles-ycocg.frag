#version 330

uniform sampler2D atlasTexture;
uniform sampler2D atlasAlpha;
uniform sampler2D mask1Texture;
uniform sampler2D mask2Texture;

in vec2 vUV;
in vec4 maskUVs;
layout(location = 0) out vec4 fragColor;
in vec3 vTint;
flat in uint vFlags;

vec4 YCoCgToRGB(vec4 ycocg, float alpha)
{
    float Y = ycocg.w;
    float scale = 1.0 / ((31.875 * ycocg.z) + 1.0);
    float Co = (ycocg.x - 0.501960813999176025390625) * scale;
    float Cg = (ycocg.y - 0.501960813999176025390625) * scale;
    float R = (Y + Co) - Cg;
    float G = Y + Cg;
    float B = (Y - Co) - Cg;
    return vec4(R, G, B, alpha);
}

void main()
{
    vec4 yCoCg = texture(atlasTexture, vUV);
    float alpha = texture(atlasAlpha, vUV).x;
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
    vec4 param = yCoCg;
    float param_1 = alpha;
    fragColor = YCoCgToRGB(param, param_1) * mask;
    vec3 _133 = fragColor.xyz * vTint;
    fragColor = vec4(_133.x, _133.y, _133.z, fragColor.w);
}

