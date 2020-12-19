#include "TenCubeSpaceScene.h"
#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"
#include "Shader.h"
#include "Camera.h"

HRESULT TenCubeSpaceScene::Init()
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	unsigned int indices[] = {  // 0���� �����Ѵٴ� ���� ����ϼ���!
		0, 1, 3,   // ù ��° �ﰢ��
		1, 2, 3    // �� ��° �ﰢ��
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
	0.0f, 0.0f,  // ���� �ϴ� �𼭸�  
	1.0f, 0.0f,  // ���� �ϴ� �𼭸�
	0.5f, 1.0f   // ����� �𼭸�
	};

	glGenVertexArrays(1, &VAO);
	// 1. Vertex Array Object ���ε�
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	// 2. OpenGL�� ����ϱ� ���� vertex ����Ʈ�� vertex ���ۿ� ����
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	// 3. OpenGL�� ����ϱ� ���� �ε��� ����Ʈ�� element ���ۿ� ����
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// �ؽ�ó wrapping/filtering �ɼ� ����(���� ���ε��� �ؽ�ó ��ü�� ����)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	// ���� GL_CLAMP_TO_BORDER �ɼ��� �����ϸ� �׵θ��� ���� ���������
	/*
	float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	*/

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	// ��ҽ� �۾�
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// Ȯ��� �۾� GL_NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// Ȯ��� �۾�

	// �ؽ�ó �ε� �� ����
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

	ourShader->use(); // uniform�� �����ϱ� ���� shader�� Ȱ��ȭ�ؾ� �Ѵٴ� ���� ����������!  
	glUniform1i(glGetUniformLocation(ourShader->ID, "texture1"), 0); // ���� ����
	ourShader->setInt("texture2", 1); // Ȥ�� shader Ŭ������ Ȱ��
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
	// ..:: ����� �ڵ� (������ ���� ����) :: ..

	// colorbuffer ����
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

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

TenCubeSpaceScene::TenCubeSpaceScene()
{
}

TenCubeSpaceScene::~TenCubeSpaceScene()
{
}
