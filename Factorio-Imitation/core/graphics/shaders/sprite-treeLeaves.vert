#version 330

layout(std140) uniform vsConstants
{
    mat4 projection;
} _19;

uniform samplerBuffer distortionUVLookup;

layout(location = 0) in vec3 position;
out vec2 vUVTexture;
layout(location = 1) in vec2 uvTexture;
out vec4 vTint;
layout(location = 2) in vec4 tint;
layout(location = 3) in int uvLookupIndex;
out vec2 vUVDistortion;

void main()
{
    gl_Position = _19.projection * vec4(position, 1.0);
    vUVTexture = uvTexture;
    vTint = tint;
    uint vertexID = uint(gl_VertexID) % 4u;
    uint i = vertexID / 2u;
    uint j = vertexID % 2u;
    vec2 corner = vec2(float(i), float(j));
    if (uvLookupIndex != 0)
    {
        vec4 uvCoords = texelFetch(distortionUVLookup, uvLookupIndex - 1);
        vUVDistortion = uvCoords.xy + (corner * uvCoords.zw);
    }
    else
    {
        vUVDistortion = vec2(-1.0);
    }
}

