#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
};

in UIVertexShaderOut
{
	vec2 TexCoords;
} IN;

uniform float alpha;
uniform Material material;

void main()
{
    FragColor = texture(material.diffuse, IN.TexCoords) * vec4(1.0f, 1.0f, 1.0f, alpha);
}  
