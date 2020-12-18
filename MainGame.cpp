#include "MainGame.h"
#include "Image.h"
#include "TitleScene.h"
#include "TileMapToolScene.h"
#include "LoadingScene1.h"
#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"
#include "Shader.h"

HRESULT MainGame::Init()
{
	hdc = GetDC(g_hWnd);
	
	backBuffer = new Image();
	backBuffer->Init(WINSIZE_X, WINSIZE_Y);

	SetWindowSize((1920 - WINSIZE_X)/2, (1080 - WINSIZE_Y) / 2,  WINSIZE_X, WINSIZE_Y);
	ShowCursor(false);

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
	glEnable(GL_DEPTH_TEST);

	unsigned int indices[] = {  // 0부터 시작한다는 것을 명심하세요!
	0, 1, 3,   // 첫 번째 삼각형
	1, 2, 3    // 두 번째 삼각형
	};
	
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	
	float texCoords[] = {
	0.0f, 0.0f,  // 좌측 하단 모서리  
	1.0f, 0.0f,  // 우측 하단 모서리
	0.5f, 1.0f   // 꼭대기 모서리
	};

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
	
	// 위치 attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 텍스쳐 attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// 5. 오브젝트를 그리고 싶을 때 우리가 생성한 shader program을 사용
	ourShader = new Shader("SimpleVertexShader.glsl", "SimpleFragmentShader.glsl");

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// 텍스처 wrapping/filtering 옵션 설정(현재 바인딩된 텍스처 객체에 대해)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	// 만약 GL_CLAMP_TO_BORDER 옵션을 선택하면 테두리의 색을 정해줘야함
	/* 
	float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	*/

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	// 축소시 작업
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// 확대시 작업 GL_NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// 확대시 작업

	// 텍스처 로드 및 생성
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD); // this is default
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);

	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	unsigned char* data = stbi_load("graphics/entity/assembling-machine-1/assembling-machine-1.png", &width, &height, &nrChannels, STBI_rgb_alpha);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	data = stbi_load("graphics/entity/artillery-turret/hr-artillery-turret-base.png", &width, &height, &nrChannels, STBI_rgb_alpha);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	ourShader->use(); // uniform을 설정하기 전에 shader를 활성화해야 한다는 것을 잊지마세요!  
	glUniform1i(glGetUniformLocation(ourShader->ID, "texture1"), 0); // 직접 설정
	ourShader->setInt("texture2", 1); // 혹은 shader 클래스를 활용
	ourShader->setFloat("radio", radio);
	stbi_image_free(data);

	return S_OK;
}

void MainGame::Release()
{
	delete ourShader;

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

	float cameraSpeed = 1.0f * TimerManager::GetSingleton()->GetTimeElapsed(); // adjust accordingly

	if (KeyManager::GetSingleton()->IsStayKeyDown('W'))
		cameraPos += cameraSpeed * cameraFront;
	if (KeyManager::GetSingleton()->IsStayKeyDown('S'))
		cameraPos -= cameraSpeed * cameraFront;
	if (KeyManager::GetSingleton()->IsStayKeyDown('A'))
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (KeyManager::GetSingleton()->IsStayKeyDown('D'))
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_UP))
	{
		radio = Clamp(radio + 0.1f, 0.0f, 1.0f);
		ourShader->setFloat("radio", radio);
	}
	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_DOWN))
	{
		radio = Clamp(radio - 0.1f, 0.0f, 1.0f);
		ourShader->setFloat("radio", radio);
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

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), float(WINSIZE_X) / WINSIZE_Y, 0.1f, 100.0f);

	// camera vecs
	glm::mat4 view;
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	ourShader->setMat4("view", view);
	ourShader->setMat4("projection", projection);
	
	RECT rc2;
	POINT lt, rb;
	GetClientRect(g_hWnd, &rc2);
	// 클라이언트 크기를 받아옴
	lt.x = rc2.left;
	lt.y = rc2.top;
	rb.x = rc2.right;
	rb.y = rc2.bottom; 
	// 받아온 클라이언트 크기를좌표로 입력
	ClientToScreen(g_hWnd, &lt);
	ClientToScreen(g_hWnd, &rb);
	// 클라이언트 내 좌표를 윈도우상 좌표로 변환
	rc2.left = lt.x; 
	rc2.top = lt.y; 
	rc2.right = rb.x; 
	rc2.bottom = rb.y;

	float xoffset = g_ptMouse.x - WINSIZE_X / 2;
	float yoffset = - g_ptMouse.y + WINSIZE_Y / 2; // y 좌표의 범위는 밑에서부터 위로가기 때문에 반대로 바꿉니다.
	SetCursorPos(lt.x + WINSIZE_X / 2, lt.y + WINSIZE_Y / 2);
	float sensitivity = 0.01f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	
	yaw += xoffset;
	pitch = Clamp(pitch + yoffset, -89.0f, 89.0f);

	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cameraFront = glm::normalize(front);
	
	InvalidateRect(g_hWnd, NULL, false);
}

void MainGame::Render()
{
	//HDC backDC = backBuffer->GetMemDC();

	//SceneManager::GetSingleton()->Render(backDC);
	TimerManager::GetSingleton()->Render(hdc);
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
	
	*/
	wsprintf(szText, "g_mousezDelta : %d", int(g_mousezDelta));
	TextOut(hdc, WINSIZE_X - 400, 60, szText, strlen(szText));
	

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	
	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	// shader를 활성화
	ourShader->use();

	/*
	// uniform 컬러 수정
	float timeValue = timeGetTime()/1000.0f;
	float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
	int vertexColorLocation = glGetUniformLocation(programID, "ourColor");
	glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
	*/

	glm::vec3 cubePositions[] = 
	{
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	glBindVertexArray(VAO);
	for (unsigned int i = 0; i < 10; i++)
	{
		glm::mat4 model;
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i;
		model = glm::rotate(model, glm::radians(angle) * (float)timeGetTime() / 1000.0f, glm::vec3(1.0f, 0.3f, 0.5f));
		ourShader->setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}


	glBindVertexArray(VAO);
	// glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	// glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
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

	case WM_MOUSEWHEEL:
		g_mousezDelta += GET_WHEEL_DELTA_WPARAM(wParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}
