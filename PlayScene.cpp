#include "PlayScene.h"
#include "CharacterUI.h"
#include "Character.h"
#include "Texture.h"
#include "GLImage.h"
#include "Shader.h"
#include "Camera.h"
#include "Tile.h"
#include "TileRenderer.h"
#include "TextRenderer.h"

HRESULT PlayScene::Init()
{
    SetWindowSize((1920 - width) / 2, (1080 - height) / 2, width, height);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD); // this is default
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // player
    player = new Character();
    player->Init();

    tileRenderer = new TileRenderer();
    tileRenderer->Init();

    // camera
    camera = new Camera();
    camera->Init();
    camera->SetPosition({ 0.0f, 0.0f, 1.0f });
    camera->SetTarget(player->GetLpPosition());

	// build and compile our shader zprogram
	// ------------------------------------
	lightingShader = new Shader("StandardVertexShader.glsl", "StandardFragmentShader.glsl");
    lightingShader->setInt("material.diffuse", 0);
    UIShader = new Shader("UIVertexShader.glsl", "UIFragmentShader.glsl");
    UIShader->setInt("material.diffuse", 0);

    characterUI = new CharacterUI();
    characterUI->Init();
    characterUI->SetLocalPosition(glm::vec2(width / 2, height / 2));

	// shader configuration
	// --------------------
    UIShader->use();

    glm::mat4 projection = glm::ortho(0.0f, 1600.0f, 0.0f, 900.0f, -1.0f, 1.0f);
    UIShader->setMat4("projection", projection);

    glm::vec3 pointLightPositions[] = {
    glm::vec3(   0.0f,     0.0f, 0.1f),
    glm::vec3(1000.0f,     0.0f, 0.1f),
    glm::vec3(   0.0f,  1000.0f, 0.1f),
    glm::vec3(1000.0f,  1000.0f, 0.1f),
    };

	lightingShader->use();
    // change the light's position values over time (can be done anywhere in the render loop actually, but try to do it at least before using the light source positions)
    lightingShader->setFloat("material.shininess", 64.0f);
    Texture* white = TextureManager::GetSingleton()->FindTexture("White");
    lightingShader->setInt("material.specular", 128);
    glActiveTexture(GL_TEXTURE0 + 16);
    glBindTexture(GL_TEXTURE_2D, white->GetID());

    // directional light(static)
    lightingShader->setVec3("dirLight.direction", { 0.5f, 0.5f, 0.3f });
    lightingShader->setVec3("dirLight.ambient", { 0.1f, 0.1f, 0.1f });
    lightingShader->setVec3("dirLight.diffuse", { 0,0,0 });
    lightingShader->setVec3("dirLight.specular", { 0.0f, 0.0f, 0.0f });

    // player point light(static)
    lightingShader->setVec3("pointLights[0].ambient", { 0.05f, 0.05f, 0.05f });
    lightingShader->setVec3("pointLights[0].diffuse", { 0.6f, 0.6f, 0.6f });
    lightingShader->setVec3("pointLights[0].specular", { 1.0f, 1.0f, 1.0f });
    lightingShader->setFloat("pointLights[0].constant", 0.08f);
    lightingShader->setFloat("pointLights[0].linear", 0.001f);
    lightingShader->setFloat("pointLights[0].quadratic", 0.0164f);

    // point light 2
    lightingShader->setVec3("pointLights[1].position", pointLightPositions[1]);
    lightingShader->setVec3("pointLights[1].ambient", { 0.05f, 0.05f, 0.05f });
    lightingShader->setVec3("pointLights[1].diffuse", { 0.8f, 0.8f, 0.8f });
    lightingShader->setVec3("pointLights[1].specular", { 1.0f, 1.0f, 1.0f });
    lightingShader->setFloat("pointLights[1].constant", 1.0f * 0.1);
    lightingShader->setFloat("pointLights[1].linear", 0.09);
    lightingShader->setFloat("pointLights[1].quadratic", 0.032);

    // point light 3
    lightingShader->setVec3("pointLights[2].position", pointLightPositions[2]);
    lightingShader->setVec3("pointLights[2].ambient", { 0.05f, 0.05f, 0.05f });
    lightingShader->setVec3("pointLights[2].diffuse", { 0.8f, 0.8f, 0.8f });
    lightingShader->setVec3("pointLights[2].specular", { 1.0f, 1.0f, 1.0f });
    lightingShader->setFloat("pointLights[2].constant", 1.0f * 0.1);
    lightingShader->setFloat("pointLights[2].linear", 0.09);
    lightingShader->setFloat("pointLights[2].quadratic", 0.032);

    // point light 4
    lightingShader->setVec3("pointLights[3].position", pointLightPositions[3]);
    lightingShader->setVec3("pointLights[3].ambient", { 0.05f, 0.05f, 0.05f });
    lightingShader->setVec3("pointLights[3].diffuse", { 0.8f, 0.8f, 0.8f });
    lightingShader->setVec3("pointLights[3].specular", { 1.0f, 1.0f, 1.0f });
    lightingShader->setFloat("pointLights[3].constant", 1.0f * 0.1);
    lightingShader->setFloat("pointLights[3].linear", 0.09);
    lightingShader->setFloat("pointLights[3].quadratic", 0.032);

    // TextRenderer Init
    textRenderer = new TextRenderer();
    textRenderer->Init(1600, 900);
    textRenderer->Load("Fonts/NotoSans-Regular.ttf", 24);

	return S_OK;
}

void PlayScene::Release()
{
    SAFE_RELEASE(textRenderer);
    SAFE_RELEASE(tileRenderer);
    SAFE_RELEASE(characterUI);
    SAFE_RELEASE(player);
    SAFE_DELETE(UIShader);
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
    if (KeyManager::GetSingleton()->IsOnceKeyDown('E'))
    {
        characterUI->SetActive(!characterUI->IsActive());
    }

    tileRenderer->Update();

    player->Update();
    
	camera->Update();
    characterUI->Update();
}

void PlayScene::Render(HDC hdc)
{
	glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // be sure to activate shader when setting uniforms/drawing objects
    lightingShader->use();
    // player point light(dynamic)
    lightingShader->setVec3("pointLights[0].position", glm::vec3(player->GetLpPosition()->x, player->GetLpPosition()->y, 0.01f));
    // directional light(dynamic)
    lightingShader->setVec3("dirLight.diffuse", glm::vec3((sin(timeGetTime() / 3000.0f) + 1) / 2));

    // view/projection transformations
    //glm::mat4 projection = glm::ortho(float(-width) / 2, float(width) / 2, float(-height) / 2.0f, float(height) / 2, -1.0f, 1.0f);
    glm::mat4 projection = glm::ortho(0.0f, float(width) / camera->GetZoom(), 0.0f, float(height) / camera->GetZoom(), -1.0f, 1.0f);
    glm::mat4 view = camera->GetViewMatrix();
    view = glm::translate(view, glm::vec3(float(width) / camera->GetZoom() / 2, float(height) / camera->GetZoom() / 2, 0.0f));
    lightingShader->setMat4("projection", projection);
    lightingShader->setMat4("view", view);

    UIShader->use();
    glm::mat4 UIprojection = glm::ortho(0.0f, float(width), 0.0f, float(height), -1.0f, 1.0f);
    UIShader->setMat4("projection", UIprojection);

    tileRenderer->Render(lightingShader);

    player->Render(lightingShader);
    characterUI->Render(UIShader);

    textRenderer->RenderText("g_ptMouse : " + to_string(g_ptMouse.x) + ", " + to_string(g_ptMouse.y), 0, 0, 1.0f);
    textRenderer->RenderText("g_ptMouse + cameraPos : " + to_string(g_ptMouse.x + camera->GetPosition().x) + ", " + to_string(g_ptMouse.y + camera->GetPosition().y), 0, 24, 1.0f);

	glFlush();
}

PlayScene::~PlayScene()
{
}
