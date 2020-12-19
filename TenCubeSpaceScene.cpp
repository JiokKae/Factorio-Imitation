#include "TenCubeSpaceScene.h"
#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"
#include "Shader.h"
#include "Camera.h"

HRESULT TenCubeSpaceScene::Init()
{
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

	camera = new Camera();
	camera->Init();

	return S_OK;
}

void TenCubeSpaceScene::Release()
{
	SAFE_RELEASE(camera);
	SAFE_DELETE(ourShader);
}

void TenCubeSpaceScene::Update()
{
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

	if (KeyManager::GetSingleton()->IsStayKeyDown(VK_F6))
	{
		TimerManager::GetSingleton()->SetTargetFPS(60);
	}
	if (KeyManager::GetSingleton()->IsStayKeyDown(VK_ESCAPE))
	{
		SceneManager::GetSingleton()->ChangeScene("TitleScene");
	}

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(camera->GetFov()), float(WINSIZE_X) / WINSIZE_Y, 0.1f, 100.0f);

	// camera vecs
	camera->Update();
	glm::mat4 view;
	view = camera->GetViewMatrix();

	ourShader->setMat4("view", view);
	ourShader->setMat4("projection", projection);
}

void TenCubeSpaceScene::Render(HDC hdc)
{
	// ..:: 드로잉 코드 (렌더링 루프 내부) :: ..

	// colorbuffer 비우기
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	// shader를 활성화
	ourShader->use();

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

	glBindVertexArray(0);

	glFlush();
}

TenCubeSpaceScene::TenCubeSpaceScene()
{
}

TenCubeSpaceScene::~TenCubeSpaceScene()
{
}
