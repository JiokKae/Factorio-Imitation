#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

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
	o.TexCoords = (aTexCoords + currFrame) / maxFrame;
	gl_Position = model * vec4(aPos, 1.0); // projection * view * model * vec4(aPos, 1.0);
}