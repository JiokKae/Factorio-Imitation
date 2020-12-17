#version 330 core
layout (location = 0) in vec3 aPos; // ��ġ ������ attribute position 0�� �����ϴ�.
  
out vec4 vertexColor; // fragment shader�� ���� �÷� ����� ����

void main()
{
    gl_Position = vec4(aPos, 1.0); // vec4�� �����ڿ� vec3�� ���������� �־����ϴ�.
    vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // ��� ������ £�� �������� �����߽��ϴ�.
}