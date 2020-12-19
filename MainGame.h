#pragma once
#include "GameNode.h"
#include "framework.h"
class Image;
class Shader;
class Camera;
class MainGame : public GameNode
{
private:
	HDC hdc;
	HDC backDC;
	HGLRC hrc;
	Image* backBuffer;

	int Width, Height;
	GLint EnvMode = GL_REPLACE; 
	GLint TexFilter = GL_LINEAR;

	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;

	unsigned int texture1;
	unsigned int texture2;
	float radio = 0.5;

	Shader* ourShader = nullptr;
	Camera* camera = nullptr;

public:
	HRESULT Init();				// ��� ���� �ʱ�ȭ, �޸� �Ҵ�
	void Release();				// �޸� ����
	void Update();				// ������ ���� ���� ���� ���� (������ ����)
	void Render();				// ������ ���� ��� (�̹���, �ؽ�Ʈ ��)

	LRESULT MainProc(HWND hWnd, UINT iMessage,
		WPARAM wParam, LPARAM lParam);
};

