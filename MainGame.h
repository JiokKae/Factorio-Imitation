#pragma once
#include "GameNode.h"
#include "framework.h"
class Image;
class Shader;
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

	Shader* ourShader;

	unsigned int texture1;
	unsigned int texture2;
	float radio = 0.5;

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	bool firstMouse = true;
	float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
	float pitch = 0.0f;
	float fov = 45.0f;
	
public:
	HRESULT Init();				// 멤버 변수 초기화, 메모리 할당
	void Release();				// 메모리 해제
	void Update();				// 프레임 단위 게임 로직 실행 (데이터 변경)
	void Render();				// 프레임 단위 출력 (이미지, 텍스트 등)

	LRESULT MainProc(HWND hWnd, UINT iMessage,
		WPARAM wParam, LPARAM lParam);
};

