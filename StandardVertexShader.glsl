#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec2 currFrame;
uniform vec2 maxFrame;

out StandardVertexShaderOut
{
	vec3 FragPos; 
	vec3 Normal;
	vec2 TexCoords;
} o;

void main()
{
	o.FragPos = vec3(model * vec4(aPos, 1.0));
	o.Normal = mat3(transpose(inverse(model))) * aNormal;  
	o.TexCoords = (aTexCoords + currFrame) / maxFrame;

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}