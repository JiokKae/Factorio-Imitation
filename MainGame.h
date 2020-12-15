#pragma once
#include "GameNode.h"
#include "framework.h"
class Image;
class MainGame : public GameNode
{
private:
	HDC hdc;
	HDC backDC;
	HGLRC hrc;
	Image* backBuffer;

	GLubyte* data; 
	int Width, Height;
	GLfloat xAngle, yAngle, zAngle; 
	GLint EnvMode = GL_REPLACE; 
	GLint TexFilter = GL_LINEAR;

	int width, height, nrChannels;
	unsigned char* data2;

	unsigned int VBO;
	unsigned int VAO;
	float vertices[9];
	GLuint programID;

	unsigned int texture;
public:
	HRESULT Init();				// ��� ���� �ʱ�ȭ, �޸� �Ҵ�
	void Release();				// �޸� ����
	void Update();				// ������ ���� ���� ���� ���� (������ ����)
	void Render();				// ������ ���� ��� (�̹���, �ؽ�Ʈ ��)

	LRESULT MainProc(HWND hWnd, UINT iMessage,
		WPARAM wParam, LPARAM lParam);
};

