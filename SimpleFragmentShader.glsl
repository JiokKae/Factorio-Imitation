
#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float radio;
void main()
{
    vec4 a = {1,0,0,1};
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), radio);
}