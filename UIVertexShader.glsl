#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec2 currFrame;
uniform vec2 maxFrame;

out UIVertexShaderOut
{
	vec2 TexCoords;
} o;

void main()
{
	o.TexCoords = (vertex.zw + currFrame) / maxFrame;
	gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}