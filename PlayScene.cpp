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
#include "PointLight.h"

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
    glm::vec3(   0.0f,     0.0f, 10.0f),
    glm::vec3(1000.0f,     0.0f, 10.0f),
    glm::vec3(   0.0f,  1000.0f, -10.0f),
    glm::vec3(1000.0f,  1000.0f, -10.0f),
    };

	lightingShader->use();
    // change the light's position values over time (can be done anywhere in the render loop actually, but try to do it at least before using the light source positions)
    lightingShader->setFloat("material.shininess", 64.0f);
    Texture* white = TextureManager::GetSingleton()->FindTexture("White");
    lightingShader->setInt("material.specular", 128);
    glActiveTexture(GL_TEXTURE0 + 16);
    glBindTexture(GL_TEXTURE_2D, white->GetID());

    pointLights = new PointLight[4]();

    // directional light(static)
    lightingShader->setVec3("dirLight.direction", { 0.5f, 0.5f, 0.3f });
    lightingShader->setVec3("dirLight.ambient", { 0.01f, 0.01f, 0.01f });
    lightingShader->setVec3("dirLight.diffuse", { 0,0,0 });
    lightingShader->setVec3("dirLight.specular", { 0.0f, 0.0f, 0.0f });

    // player point light(static)
    lightingShader->setVec3("pointLights[0].ambient", { 0.01f, 0.01f, 0.01f });
    lightingShader->setVec3("pointLights[0].diffuse", { 0.6f, 0.6f, 0.6f });
    lightingShader->setFloat("pointLights[0].constant", 0.5f);
    lightingShader->setFloat("pointLights[0].linear", 0.06f);
    lightingShader->setFloat("pointLights[0].quadratic", 0.082f);

    // point light 2
    pointLights[1].position = glm::vec3(1000.0f, 0.0f, 0.1f);
    lightingShader->setVec3("pointLights[1].position", pointLightPositions[1]);
    lightingShader->setVec3("pointLights[1].ambient", { 0.01f, 0.01f, 0.01f });
    lightingShader->setVec3("pointLights[1].diffuse", { 0.8f, 0.8f, 0.8f });
    lightingShader->setFloat("pointLights[1].constant", 1.0f);
    lightingShader->setFloat("pointLights[1].linear", 0.09f);
    lightingShader->setFloat("pointLights[1].quadratic", 0.062f);

    // point light 3
    lightingShader->setVec3("pointLights[2].position", pointLightPositions[2]);
    lightingShader->setVec3("pointLights[2].ambient", { 0.01f, 0.01f, 0.01f });
    lightingShader->setVec3("pointLights[2].diffuse", { 0.8f, 0.8f, 0.8f });
    lightingShader->setFloat("pointLights[2].constant", 1.0f);
    lightingShader->setFloat("pointLights[2].linear", 0.09f);
    lightingShader->setFloat("pointLights[2].quadratic", 0.062f);

    // point light 4
    lightingShader->setVec3("pointLights[3].position", pointLightPositions[3]);
    lightingShader->setVec3("pointLights[3].ambient", { 0.01f, 0.01f, 0.01f });
    lightingShader->setVec3("pointLights[3].diffuse", { 0.8f, 0.8f, 0.8f });
    lightingShader->setFloat("pointLights[3].constant", 1.0f);
    lightingShader->setFloat("pointLights[3].linear", 0.09f);
    lightingShader->setFloat("pointLights[3].quadratic", 0.062f);

    // TextRenderer Init
    textRenderer = new TextRenderer();
    textRenderer->Init(1600, 900);
    textRenderer->Load("Fonts/NotoSans-Regular.ttf", 24);

    glGenBuffers(1, &uboMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));

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
    lightingShader->setVec3("dirLight.diffuse", glm::vec3((sin(timeGetTime() / 3000.0f) + 1) / 2) * 0.0f);

    // 뷰/프로젝션 매트릭스 연산
    glm::mat4 projection = glm::ortho(0.0f, float(width) / camera->GetZoom(), 0.0f, float(height) / camera->GetZoom());
    glm::mat4 view = camera->GetViewMatrix();
    view = glm::translate(view, glm::vec3(float(width) / camera->GetZoom() / 2, float(height) / camera->GetZoom() / 2, 0.0f));

    // uniform buffer object 0번 바인딩 인덱스에 프로젝션과 뷰 매트릭스 설정
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    UIShader->use();
    glm::mat4 UIprojection = glm::ortho(0.0f, float(width), 0.0f, float(height));
    UIShader->setMat4("projection", UIprojection);

    tileRenderer->Render(lightingShader);

    player->Render(lightingShader);
    characterUI->Render(UIShader);

    textRenderer->RenderText("FPS: " + to_string(TimerManager::GetSingleton()->GetFPS()),
        0, 10);
    textRenderer->RenderText("g_ptMouse : " + to_string(g_ptMouse.x) + ", " + to_string(g_ptMouse.y), 
        0, 40);
    textRenderer->RenderText("cameraPos : " + to_string(camera->GetPosition().x) + ", " + to_string(camera->GetPosition().y),
        0, 70);

    char str[128];
    sprintf_s(str, "Cursor: (%.1f, %.1f)", 
        (g_ptMouse.x - width / 2) / (64 * camera->GetZoom()) + (camera->GetPosition().x / 64), 
        (g_ptMouse.y - height / 2) / (64 * camera->GetZoom()) + (camera->GetPosition().y / 64));
    textRenderer->RenderText(string(str), 0, 100);

    textRenderer->RenderText("Zoom: " + to_string(camera->GetZoom()),
        0, 130);
	glFlush();
}

PlayScene::~PlayScene()
{
}
