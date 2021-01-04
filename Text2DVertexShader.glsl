#version 420 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

layout (std140, binding = 2) uniform UIMatrices
{
	mat4 projection;
    mat4 reverseVerticalProjection;
};

void main()
{    
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
} 