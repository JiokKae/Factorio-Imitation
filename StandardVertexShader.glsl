#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec2 currFrame;
uniform vec2 maxFrame;

out StandardVertexShaderOut
{
	vec3 FragPos; 
	vec2 TexCoords;
} o;

void main()
{
	o.FragPos = vec3(model * vec4(vertex.xy, 0.0f, 1.0));
	o.TexCoords = (vertex.zw + currFrame) / maxFrame;

	gl_Position = projection * view * model * vec4(vertex.xy, 0.0f, 1.0);
}