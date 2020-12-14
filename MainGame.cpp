#include "MainGame.h"
#include "Image.h"
#include "TitleScene.h"
#include "TileMapToolScene.h"
#include "LoadingScene1.h"
#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"

HRESULT MainGame::Init()
{
	hdc = GetDC(g_hWnd);
	
	backBuffer = new Image();
	backBuffer->Init(WINSIZE_X, WINSIZE_Y);

	// OpenGL
	PIXELFORMATDESCRIPTOR pfd;
	int nPixelFormat;
	memset(&pfd, 0, sizeof(pfd));

	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |  PFD_DOUBLEBUFFER_DONTCARE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;

	nPixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, nPixelFormat, &pfd);

	hrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hrc);

	TimerManager::GetSingleton()->Init();
	SceneManager::GetSingleton()->Init();

	// 씬 추가
	SceneManager::GetSingleton()->AddScene("TitleScene", new TitleScene());
	SceneManager::GetSingleton()->AddScene("TileMapToolScene", new TileMapToolScene());
	SceneManager::GetSingleton()->AddLoadingScene("LoadingScene1", new LoadingScene1());

	SceneManager::GetSingleton()->ChangeScene("TitleScene");




	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// 텍스처 wrapping/filtering 옵션 설정(현재 바인딩된 텍스처 객체에 대해)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("graphics/entity/assembling-machine-1/assembling-machine-1.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		;//std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


	return S_OK;
}

void MainGame::Release()
{
	// OpenGL
	wglMakeCurrent(hdc, NULL);
	wglDeleteContext(hrc);

	SAFE_RELEASE(backBuffer);

	SceneManager::GetSingleton()->Release();
	TimerManager::GetSingleton()->Release();

	ReleaseDC(g_hWnd, hdc);
}

void MainGame::Update()
{
	SceneManager::GetSingleton()->Update();

	InvalidateRect(g_hWnd, NULL, false);
}

void MainGame::Render()
{
	//HDC backDC = backBuffer->GetMemDC();

	//SceneManager::GetSingleton()->Render(backDC);
	
	char szText[128];

	wsprintf(szText, "X : %d, Y : %d", g_ptMouse.x, g_ptMouse.y);
	TextOut(hdc, 10, 5, szText, strlen(szText));
	wsprintf(szText, "g_time : %d", (int)g_time);
	TextOut(hdc, WINSIZE_X - 300, 0, szText, strlen(szText));

	TimerManager::GetSingleton()->Render(hdc);


	float vertices[] = {
		// 위치              // 컬러             // 텍스처 좌표
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 우측 상단
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 우측 하단
	   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 좌측 하단
	   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 좌측 상단
	};

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);



	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.5 + sin(g_time) * 0.5f, 1.0, 1.0);
	glBegin(GL_POLYGON);
	{
		glVertex3f(0.25 + sin(g_time) * 0.5f, 0.25, 0.0);
		glVertex3f(0.75, 0.25, 0.0);
		glVertex3f(0.75, 0.75, 0.0);
		glVertex3f(0.25, 0.75, 0.0);
	}



	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnd();

	glFlush();





	// 백버퍼 복사(출력)
	//backBuffer->Render(hdc, 0, 0, WINSIZE_X, WINSIZE_Y);
}

LRESULT MainGame::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_MOUSEMOVE:
		g_ptMouse.x = LOWORD(lParam);
		g_ptMouse.y = HIWORD(lParam);
		break;

	case WM_SIZE:
		glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glOrtho(-1, 1, -1, 1, 1, -1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}
