#version 330

struct TurretRangeData
{
    vec4 posAndRadius;
    vec4 rangeBoundNormals;
};

layout(std140) uniform turretRangeData
{
    int dataSize;
    vec4 color;
    TurretRangeData data[511];
} _28;

uniform sampler2D pretest;

layout(location = 0) out vec4 fragColor;
in vec2 vUV;

void main()
{
    fragColor = vec4(0.0);
    for (int i = 0; i < _28.dataSize; i++)
    {
        vec2 pos = _28.data[i].posAndRadius.xy;
        vec2 startVec = _28.data[i].rangeBoundNormals.xy;
        vec2 endVec = _28.data[i].rangeBoundNormals.zw;
        vec2 delta = gl_FragCoord.xy - pos;
        vec2 normal = vec2(delta.y, -delta.x);
        float dist = dot(delta, delta);
        float crossA = dot(startVec, normal);
        float crossB = dot(endVec, normal);
        float maxRadiusSquared = _28.data[i].posAndRadius.z;
        float minRadiusSquared = _28.data[i].posAndRadius.w;
        if (all(bvec4(crossA >= 0.0, crossB <= 0.0, dist < maxRadiusSquared, dist > minRadiusSquared)))
        {
            fragColor = _28.color;
            break;
        }
    }
}

