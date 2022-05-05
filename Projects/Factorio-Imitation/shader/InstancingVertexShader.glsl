#version 420 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
layout (location = 1) in vec2 instanceCurrFrame;
layout (location = 2) in vec2 instanceOffset;

layout (std140, binding = 0) uniform Matrices
{
	mat4 projection;
	mat4 view;	
};

uniform vec2 maxFrame;
uniform mat4 model;
uniform vec2 offset;
uniform vec2 margin;
uniform float vertexScale;

out StandardVertexShaderOut
{
	vec3 FragPos; 
	vec2 TexCoords;
} o;

void main()
{
	o.FragPos = vec3(model * vec4(vertex.xy * vertexScale + instanceOffset + offset, 0.0f, 1.0));
	
	o.TexCoords = (vertex.zw * (1 - margin * 2) + margin + instanceCurrFrame) / maxFrame;

	gl_Position = projection * view * model * vec4(vertex.xy * vertexScale + instanceOffset + offset, 0.0f, 1.0);
}