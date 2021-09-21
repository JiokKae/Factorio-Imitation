#version 420 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>

layout (std140, binding = 0) uniform Matrices
{
	mat4 projection;
	mat4 view;
};

uniform mat4 model;
uniform vec2 currFrame;
uniform vec2 maxFrame;
uniform vec2 margin;

out StandardVertexShaderOut
{
	vec3 FragPos; 
	vec2 TexCoords;
} o;

void main()
{
	o.FragPos = vec3(model * vec4(vertex.xy, 0.0f, 1.0));
	o.TexCoords = (vertex.zw * (1 - margin * 2) + margin + currFrame) / maxFrame;
	gl_Position = projection * view * model * vec4(vertex.xy, 0.0f, 1.0);
}