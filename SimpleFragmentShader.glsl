
#version 330 core
out vec4 FragColor;
  
uniform vec4 ourColor; // OpenGL 코드에서 이 변수를 설정할 것입니다.

void main()
{
    FragColor = ourColor;
} 
