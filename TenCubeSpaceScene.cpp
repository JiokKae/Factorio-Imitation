#include "TenCubeSpaceScene.h"
#include "Shader.h"
#include "FreeCamera.h"
#include "Texture.h"

HRESULT TenCubeSpaceScene::Init()
{
	SetWindowSize((1920 - WINSIZE_X) / 2, (1080 - WINSIZE_Y) / 2, WINSIZE_X, WINSIZE_Y);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD); // this is default
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

	glGenVertexArrays(1, &VAO);
	// 1. Vertex Array Object ���ε�
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	// 2. OpenGL�� ����ϱ� ���� vertex ����Ʈ�� vertex ���ۿ� ����
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// ��ġ attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// �ؽ��� attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// 5. ������Ʈ�� �׸��� ���� �� �츮�� ������ shader program�� ���
	ourShader = new Shader("SimpleVertexShader.glsl", "SimpleFragmentShader.glsl");

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// �ؽ�ó �ε� �� ����
	texture1 = new Texture();
	texture1->Init("base/graphics/entity/assembling-machine-1/assembling-machine-1.png");
	texture2 = new Texture();
	texture2->Init("base/graphics/entity/artillery-turret/hr-artillery-turret-base.png");

	ourShader->use(); // uniform�� �����ϱ� ���� shader�� Ȱ��ȭ�ؾ� �Ѵٴ� ���� ����������!  
	ourShader->setInt("texture1", texture1->GetID());
	ourShader->setInt("texture2", texture2->GetID());
	ourShader->setFloat("radio", radio);

	camera = new FreeCamera();
	camera->Init();

	return S_OK;
}

void TenCubeSpaceScene::Release()
{
	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	SAFE_RELEASE(texture1);
	SAFE_RELEASE(texture2);
	SAFE_RELEASE(camera);
	SAFE_DELETE(ourShader);
}

void TenCubeSpaceScene::Update()
{
	if (KeyManager::GetSingleton()->IsStayKeyDown(VK_ESCAPE))
	{
		SceneManager::GetSingleton()->ChangeScene("TitleScene");
		return;
	}

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
	// ..:: ����� �ڵ� (������ ���� ����) :: ..

	// colorbuffer ����
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0 + texture1->GetID());
	glBindTexture(GL_TEXTURE_2D, texture1->GetID());
	glActiveTexture(GL_TEXTURE0 + texture2->GetID());
	glBindTexture(GL_TEXTURE_2D, texture2->GetID());

	// shader�� Ȱ��ȭ
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

TenCubeSpaceScene::~TenCubeSpaceScene()
{
}
