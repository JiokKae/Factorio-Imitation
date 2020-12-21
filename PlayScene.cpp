#include "PlayScene.h"
#include "Texture.h"
#include "GLImage.h"
#include "Shader.h"
#include "Camera.h"

HRESULT PlayScene::Init()
{
    SetWindowSize((1920 - width) / 2, (1080 - height) / 2, width, height);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD); // this is default
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // camera
    camera = new Camera();
    camera->Init();
    camera->SetPosition({ 0.0f, 0.0f, 5.0f });

	// build and compile our shader zprogram
	// ------------------------------------
	lightingShader = new Shader("StandardVertexShader.glsl", "StandardFragmentShader.glsl");


	// 텍스처 로드 및 이미지 생성
    dirt_1Image = new GLImage();
    dirt_1Image->Init(TextureManager::GetSingleton()->FindTexture("Dirt_1"), 2048, 288, 64, 9);
    
	// shader configuration
	// --------------------
    glm::vec3 pointLightPositions[] = {
    glm::vec3(0.7f,  0.2f, 0.1f),
    glm::vec3(2.3f, -3.3f, 0.1f),
    glm::vec3(-4.0f,  2.0f, 0.1f),
    glm::vec3(0.0f,  0.0f, 0.1f)
    };

	lightingShader->use();
    // change the light's position values over time (can be done anywhere in the render loop actually, but try to do it at least before using the light source positions)
    lightingShader->setFloat("material.shininess", 64.0f);
    Texture* white = TextureManager::GetSingleton()->FindTexture("White");
    lightingShader->setInt("material.specular", white->GetID());
    glActiveTexture(GL_TEXTURE0 + white->GetID());
    glBindTexture(GL_TEXTURE_2D, white->GetID());

    // directional light
    lightingShader->setVec3("dirLight.direction", { 0.5f, 0.5f, -0.3f });
    lightingShader->setVec3("dirLight.ambient", { 0.2f, 0.2f, 0.2f });
    lightingShader->setVec3("dirLight.diffuse", { 0.5f, 0.5f, 0.5f });
    lightingShader->setVec3("dirLight.specular", { 0.5f, 0.5f, 0.5f });
    // point light 1
    lightingShader->setVec3("pointLights[0].position", pointLightPositions[0]);
    lightingShader->setVec3("pointLights[0].ambient", { 0.05f, 0.05f, 0.05f });
    lightingShader->setVec3("pointLights[0].diffuse", { 0.8f, 0.8f, 0.8f });
    lightingShader->setVec3("pointLights[0].specular", { 1.0f, 1.0f, 1.0f });
    lightingShader->setFloat("pointLights[0].constant", 1.0f);
    lightingShader->setFloat("pointLights[0].linear", 0.09);
    lightingShader->setFloat("pointLights[0].quadratic", 0.032);
    // point light 2
    lightingShader->setVec3("pointLights[1].position", pointLightPositions[1]);
    lightingShader->setVec3("pointLights[1].ambient", { 0.05f, 0.05f, 0.05f });
    lightingShader->setVec3("pointLights[1].diffuse", { 0.8f, 0.8f, 0.8f });
    lightingShader->setVec3("pointLights[1].specular", { 1.0f, 1.0f, 1.0f });
    lightingShader->setFloat("pointLights[1].constant", 1.0f);
    lightingShader->setFloat("pointLights[1].linear", 0.09);
    lightingShader->setFloat("pointLights[1].quadratic", 0.032);
    // point light 3
    lightingShader->setVec3("pointLights[2].position", pointLightPositions[2]);
    lightingShader->setVec3("pointLights[2].ambient", { 0.05f, 0.05f, 0.05f });
    lightingShader->setVec3("pointLights[2].diffuse", { 0.8f, 0.8f, 0.8f });
    lightingShader->setVec3("pointLights[2].specular", { 1.0f, 1.0f, 1.0f });
    lightingShader->setFloat("pointLights[2].constant", 1.0f);
    lightingShader->setFloat("pointLights[2].linear", 0.09);
    lightingShader->setFloat("pointLights[2].quadratic", 0.032);
    // point light 4
    lightingShader->setVec3("pointLights[3].position", pointLightPositions[3]);
    lightingShader->setVec3("pointLights[3].ambient", { 0.05f, 0.05f, 0.05f });
    lightingShader->setVec3("pointLights[3].diffuse", { 0.8f, 0.8f, 0.8f });
    lightingShader->setVec3("pointLights[3].specular", { 1.0f, 1.0f, 1.0f });
    lightingShader->setFloat("pointLights[3].constant", 1.0f);
    lightingShader->setFloat("pointLights[3].linear", 0.09);
    lightingShader->setFloat("pointLights[3].quadratic", 0.032);

    // spotLight
    lightingShader->setVec3("spotLight.ambient", { 0.0f, 0.0f, 0.0f });
    lightingShader->setVec3("spotLight.diffuse", { 1.0f, 1.0f, 1.0f });
    lightingShader->setVec3("spotLight.specular", { 1.0f, 1.0f, 1.0f });
    lightingShader->setFloat("spotLight.constant", 1.0f);
    lightingShader->setFloat("spotLight.linear", 0.09);
    lightingShader->setFloat("spotLight.quadratic", 0.032);
    lightingShader->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5)));
    lightingShader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(30.0f)));

	return S_OK;
}

void PlayScene::Release()
{
    SAFE_RELEASE(dirt_1Image);
	SAFE_DELETE(lightingShader);
	SAFE_RELEASE(camera);
}

void PlayScene::Update()
{
	if (KeyManager::GetSingleton()->IsStayKeyDown(VK_ESCAPE))
	{
		SceneManager::GetSingleton()->ChangeScene("TitleScene");
		return;
	}

	camera->Update();
}

void PlayScene::Render(HDC hdc)
{
	glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // be sure to activate shader when setting uniforms/drawing objects
    lightingShader->use();
    lightingShader->setVec3("spotLight.position", camera->GetPosition());
    lightingShader->setVec3("spotLight.direction", camera->GerFront());
    lightingShader->setVec3("viewPos", camera->GetPosition());

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(camera->GetFov()), (float)width / (float)height, 0.1f, 100.0f);
    glm::mat4 view = camera->GetViewMatrix();
    lightingShader->setMat4("projection", projection);
    lightingShader->setMat4("view", view);

    for (int y = 0; y < 10; y++)
    {
        for (int x = 0; x < 10; x++)
        {
            dirt_1Image->FrameRender(lightingShader, 32 * x, 32 * y, 0, 0);
        }
    }

	glFlush();
}

PlayScene::~PlayScene()
{
}
