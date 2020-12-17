#include "MainGame.h"
#include "Image.h"
#include "TitleScene.h"
#include "TileMapToolScene.h"
#include "LoadingScene1.h"
#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"
#include <iostream>
#include <fstream>
#include <sstream>

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {

	// 쉐이더들 생성
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// 버텍스 쉐이더 코드를 파일에서 읽기
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("파일 %s 를 읽을 수 없음. 정확한 디렉토리를 사용 중입니까 ? FAQ 를 우선 읽어보는 걸 잊지 마세요!\n", vertex_file_path);
		getchar();
		return 0;
	}

	// 프래그먼트 쉐이더 코드를 파일에서 읽기
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// 버텍스 쉐이더를 컴파일
	printf("Compiling shader : %s\n", vertex_file_path);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// 버텍스 쉐이더를 검사
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// 프래그먼트 쉐이더를 컴파일
	printf("Compiling shader : %s\n", fragment_file_path);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// 프래그먼트 쉐이더를 검사
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// 프로그램에 링크
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// 프로그램 검사
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

GLubyte* LoadBmp(const char* Path, int* Width, int* Height) 
{
	HANDLE hFile; 
	DWORD FileSize, dwRead; 
	BITMAPFILEHEADER* fh = NULL; 
	BITMAPINFOHEADER* ih; 
	BYTE* pRaster; 
	hFile = CreateFileA(Path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); 
	if (hFile == INVALID_HANDLE_VALUE) 
	{ 
		return NULL; 
	} 
	FileSize = GetFileSize(hFile, NULL); 
	fh = (BITMAPFILEHEADER*)malloc(FileSize); 
	ReadFile(hFile, fh, FileSize, &dwRead, NULL); 
	CloseHandle(hFile);
	int len = FileSize - fh->bfOffBits; 
	pRaster = (GLubyte*)malloc(len); 
	memcpy(pRaster, (BYTE*)fh + fh->bfOffBits, len); // RGB로 순서를 바꾼다. 
	for (BYTE * p = pRaster; p < pRaster + len - 3; p += 3) 
	{
		BYTE b = * p; *p = * (p + 2); *(p + 2) = b; 
	} 
	ih = (BITMAPINFOHEADER *)((PBYTE)fh + sizeof(BITMAPFILEHEADER)); 
	*Width = ih -> biWidth;
	*Height = ih -> biHeight;
	free(fh); 

	return pRaster; 
}
//출처: https://202psj.tistory.com/1456?category=273755 [알레폰드의 IT 이모저모]

HRESULT MainGame::Init()
{
	hdc = GetDC(g_hWnd);
	
	backBuffer = new Image();
	backBuffer->Init(WINSIZE_X, WINSIZE_Y);

	#pragma region Win OpenGL Init
	PIXELFORMATDESCRIPTOR pfd;
	int nPixelFormat;
	memset(&pfd, 0, sizeof(pfd));

	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER_DONTCARE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;

	nPixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, nPixelFormat, &pfd);

	hrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hrc);
	#pragma endregion
	
	TimerManager::GetSingleton()->Init();
	SceneManager::GetSingleton()->Init();

	// 씬 추가
	SceneManager::GetSingleton()->AddScene("TitleScene", new TitleScene());
	SceneManager::GetSingleton()->AddScene("TileMapToolScene", new TileMapToolScene());
	SceneManager::GetSingleton()->AddLoadingScene("LoadingScene1", new LoadingScene1());

	SceneManager::GetSingleton()->ChangeScene("TitleScene");

	glewInit();
	glEnable(GL_TEXTURE_2D);
	/*
	vertices[0] = -0.5f;
	vertices[1] = -0.5f;
	vertices[2] = 0.0f;
	vertices[3] = 0.5f;
	vertices[4] = -0.5f;
	vertices[5] = 0.0f;
	vertices[6] = 0.0f;
	vertices[7] = 0.5f;
	vertices[8] = 0.0f;
	*/
	float vertices[] = {
	 0.5f,  0.5f, 0.0f,  // 우측 상단
	 0.5f, -0.5f, 0.0f,  // 우측 하단
	-0.5f, -0.5f, 0.0f,  // 좌측 하단
	-0.5f,  0.5f, 0.0f   // 좌측 상단
	};
	unsigned int indices[] = {  // 0부터 시작한다는 것을 명심하세요!
	0, 1, 3,   // 첫 번째 삼각형
	1, 2, 3    // 두 번째 삼각형
	};
	
	/*
	float vertices[] = {
		// 위치              // 컬러
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 우측 하단
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 좌측 하단
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 위 
	};
	*/
	glGenVertexArrays(1, &VAO);
	// 1. Vertex Array Object 바인딩
	glBindVertexArray(VAO);

	
	glGenBuffers(1, &VBO);
	// 2. OpenGL이 사용하기 위해 vertex 리스트를 vertex 버퍼에 복사
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	// 3. OpenGL이 사용하기 위해 인덱스 리스트를 element 버퍼에 복사
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	// 4. 그런 다음 vertex 속성 포인터를 세팅
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 5. 오브젝트를 그리고 싶을 때 우리가 생성한 shader program을 사용
	programID = LoadShaders("SimpleVertexShader.glsl", "SimpleFragmentShader.glsl");
	glUseProgram(programID);

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
	
	data = stbi_load("graphics/entity/assembling-machine-1/assembling-machine-1.png", &width, &height, &nrChannels, 0);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	
	return S_OK;
}

void MainGame::Release()
{
	#pragma region Win OpenGL Release
	wglMakeCurrent(hdc, NULL);
	wglDeleteContext(hrc);
	#pragma endregion

	SAFE_RELEASE(backBuffer);

	SceneManager::GetSingleton()->Release();
	TimerManager::GetSingleton()->Release();

	ReleaseDC(g_hWnd, hdc);
}

void MainGame::Update()
{
	SceneManager::GetSingleton()->Update();

	if (KeyManager::GetSingleton()->IsStayKeyDown('A'))
	{
		yAngle += 30 * TimerManager::GetSingleton()->GetTimeElapsed();
	}
	if (KeyManager::GetSingleton()->IsStayKeyDown('D'))
	{
		yAngle -= 30 * TimerManager::GetSingleton()->GetTimeElapsed();
	}
	if (KeyManager::GetSingleton()->IsStayKeyDown('W'))
	{
		xAngle += 30 * TimerManager::GetSingleton()->GetTimeElapsed();
	}
	if (KeyManager::GetSingleton()->IsStayKeyDown('S'))
	{
		xAngle -= 30 * TimerManager::GetSingleton()->GetTimeElapsed();
	}
	if (KeyManager::GetSingleton()->IsStayKeyDown('Q'))
	{
		zAngle += 30 * TimerManager::GetSingleton()->GetTimeElapsed();
	}
	if (KeyManager::GetSingleton()->IsStayKeyDown('E'))
	{
		zAngle -= 30 * TimerManager::GetSingleton()->GetTimeElapsed();
	}
	if (KeyManager::GetSingleton()->IsStayKeyDown('Z'))
	{
		xAngle = yAngle = zAngle = 0.0;
	}
	if (KeyManager::GetSingleton()->IsStayKeyDown(VK_F1))
	{
		EnvMode = GL_REPLACE; 
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, EnvMode);
	}
	if (KeyManager::GetSingleton()->IsStayKeyDown(VK_F2))
	{
		EnvMode = GL_MODULATE; 
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, EnvMode);
	}
	if (KeyManager::GetSingleton()->IsStayKeyDown(VK_F3))
	{
		EnvMode = GL_ADD;
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, EnvMode);
	}
	if (KeyManager::GetSingleton()->IsStayKeyDown(VK_F4))
	{
		TexFilter = GL_NEAREST;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, TexFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, TexFilter);
	}
	if (KeyManager::GetSingleton()->IsStayKeyDown(VK_F5))
	{
		TexFilter = GL_LINEAR;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, TexFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, TexFilter);
	}

	InvalidateRect(g_hWnd, NULL, false);
}

void MainGame::Render()
{
	//HDC backDC = backBuffer->GetMemDC();

	//SceneManager::GetSingleton()->Render(backDC);
	char szText[128];

	/*
	wsprintf(szText, "X : %d, Y : %d", g_ptMouse.x, g_ptMouse.y);
	TextOut(hdc, 10, 5, szText, strlen(szText));
	wsprintf(szText, "g_time : %d", (int)g_time);
	TextOut(hdc, WINSIZE_X - 400, 0, szText, strlen(szText));
	wsprintf(szText, "xAngle : %d", (int)(xAngle));
	TextOut(hdc, WINSIZE_X - 400, 20, szText, strlen(szText));
	wsprintf(szText, "yAngle : %d", (int)(yAngle));
	TextOut(hdc, WINSIZE_X - 400, 40, szText, strlen(szText));
	wsprintf(szText, "zAngle : %d", (int)(zAngle));
	TextOut(hdc, WINSIZE_X - 400, 60, szText, strlen(szText));
	TimerManager::GetSingleton()->Render(hdc);
	*/
	
	/*
	* glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
	
	glPushMatrix();
	glRotatef(xAngle, 1.0f, 0.0f, 0.0f); 
	glRotatef(yAngle, 0.0f, 1.0f, 0.0f); 
	glRotatef(zAngle, 0.0f, 0.0f, 1.0f);
	// 아랫면 흰 바닥 
	glBegin(GL_QUADS); 
	glColor3f(1, 1, 1); 
	glTexCoord2f(0.0, 1.0); 
	glVertex2f(-0.5, 0.5); 
	glTexCoord2f(1.0, 1.0);
	glVertex2f(0.5, 0.5); 
	glTexCoord2f(1.0, 0.0); 
	glVertex2f(0.5, -0.5);
	glTexCoord2f(0.0, 0.0); 
	glVertex2f(-0.5, -0.5); 
	glEnd(); 
	// 위쪽 빨간 변
	glBegin(GL_TRIANGLE_FAN); 
	glColor3f(1, 0, 0);
	glTexCoord2f(0.5, 0.5);
	glVertex3f(0.0, 0.0, -0.8);
	glTexCoord2f(1.0, 1.0);
	glVertex2f(0.5, 0.5);
	glTexCoord2f(0.0, 1.0);
	glVertex2f(-0.5, 0.5); 
	// 왼쪽 노란 변
	glColor3f(1, 1, 0); 
	glTexCoord2f(0.0, 0.0);
	glVertex2f(-0.5, -0.5); 
	// 아래쪽 초록 변 
	glColor3f(0, 1, 0); 
	glTexCoord2f(1.0, 0.0); 
	glVertex2f(0.5, -0.5); 
	// 오른쪽 파란 변 
	glColor3f(0, 0, 1);
	glTexCoord2f(1.0, 1.0); 
	glVertex2f(0.5, 0.5); 
	glEnd(); 
	glPopMatrix();
	*/

	// ..:: 드로잉 코드 (렌더링 루프 내부) :: ..

	// colorbuffer 비우기
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	
	// shader를 활성화
	glUseProgram(programID);

	// uniform 컬러 수정
	float timeValue = timeGetTime()/1000.0f;
	float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
	int vertexColorLocation = glGetUniformLocation(programID, "ourColor");
	glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glPopMatrix();
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
