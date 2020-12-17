#version 330 core
layout (location = 0) in vec3 aPos; // 위치 변수는 attribute position 0을 가집니다.
  
out vec4 vertexColor; // fragment shader를 위한 컬러 출력을 지정

void main()
{
    gl_Position = vec4(aPos, 1.0); // vec4의 생성자에 vec3를 직접적으로 주었습니다.
    vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // 출력 변수에 짙은 빨간색을 설정했습니다.
}