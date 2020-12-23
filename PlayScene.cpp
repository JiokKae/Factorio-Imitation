#include "PlayScene.h"
#include "CharacterUI.h"
#include "Character.h"
#include "Texture.h"
#include "GLImage.h"
#include "Shader.h"
#include "Camera.h"
#include "Tile.h"

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

    tiles = new Tile[30 * 30]();
    for (int y = 0; y < 30; y++)
    {
        for (int x = 0; x < 30; x++)
        {
            tiles[y * 30 + x].Init(x, y);
        }
    }

    // camera
    camera = new Camera();
    camera->Init();
    camera->SetPosition({ 0.0f, 0.0f, 25.0f });
    camera->SetTarget(player->GetLpPosition());

	// build and compile our shader zprogram
	// ------------------------------------
	lightingShader = new Shader("StandardVertexShader.glsl", "StandardFragmentShader.glsl");
    UIShader = new Shader("UIVertexShader.glsl", "UIFragmentShader.glsl");

    characterUI = new CharacterUI();
    characterUI->Init();
    
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
    lightingShader->setInt("material.specular", 128);
    glActiveTexture(GL_TEXTURE0 + 16);
    glBindTexture(GL_TEXTURE_2D, white->GetID());

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

    // spotLight top
    lightingShader->setVec3("spotLights[0].ambient", { 0.0f, 0.0f, 0.0f });
    lightingShader->setVec3("spotLights[0].diffuse", { 0.8f, 0.8f, 0.8f });
    lightingShader->setVec3("spotLights[0].specular", { 1.0f, 1.0f, 1.0f });
    lightingShader->setFloat("spotLights[0].constant", 1.0f * 0.1);
    lightingShader->setFloat("spotLights[0].linear", 0.09 * .1);
    lightingShader->setFloat("spotLights[0].quadratic", 0.032 * .1);
    lightingShader->setFloat("spotLights[0].cutOff", glm::cos(glm::radians(12.5f)));
    lightingShader->setFloat("spotLights[0].outerCutOff", glm::cos(glm::radians(15.0f)));
    // spotLight front
    lightingShader->setVec3("spotLights[1].ambient", { 0.0f, 0.0f, 0.0f });
    lightingShader->setVec3("spotLights[1].diffuse", { 1.0f, 1.0f, 1.0f });
    lightingShader->setVec3("spotLights[1].specular", { 0.0f, 0.0f, 0.0f });
    lightingShader->setFloat("spotLights[1].constant", 1.0f * 0.1);
    lightingShader->setFloat("spotLights[1].linear", 0.09 * .1);
    lightingShader->setFloat("spotLights[1].quadratic", 0.032 * .1);
    lightingShader->setFloat("spotLights[1].cutOff", glm::cos(glm::radians(12.5f)));
    lightingShader->setFloat("spotLights[1].outerCutOff", glm::cos(glm::radians(15.0f)));

	return S_OK;
}

void PlayScene::Release()
{
    
    for (int i = 0; i < 30 * 30; i++)
    {
        tiles[i].Release();
    }
    SAFE_ARR_DELETE(tiles);

    SAFE_RELEASE(characterUI);
    SAFE_RELEASE(player);
    SAFE_RELEASE(dirt_1Image);
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

    for (int i = 0; i < 30*30; i++)
    {
        tiles[i].Update();
    }

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
    lightingShader->setVec3("spotLights[0].position", camera->GetPosition());
    lightingShader->setVec3("spotLights[0].direction", camera->GerFront());
    lightingShader->setVec3("viewPos", camera->GetPosition());

    lightingShader->setVec3("spotLights[1].position",  glm::vec3(5, 5, 25.1f));// { camera->GetPosition().x, camera->GetPosition().y, 0.01f});
    lightingShader->setVec3("spotLights[1].direction", camera->GerFront());// player->GetVec3Direction());
    lightingShader->setVec3("viewPos", camera->GetPosition());



    // point light 1
    lightingShader->setVec3("pointLights[0].position", glm::vec3(player->GetLpPosition()->x/100.0f, player->GetLpPosition()->y / 100.0f, 0.1f));
    lightingShader->setVec3("pointLights[0].ambient", { 0.05f, 0.05f, 0.05f });
    lightingShader->setVec3("pointLights[0].diffuse", { 0.8f, 0.8f, 0.8f });
    lightingShader->setVec3("pointLights[0].specular", { 1.0f, 1.0f, 1.0f });
    lightingShader->setFloat("pointLights[0].constant", 1.0f * 0.1);
    lightingShader->setFloat("pointLights[0].linear", 0.09 * 0.1);
    lightingShader->setFloat("pointLights[0].quadratic", 0.032);

    // directional light
    lightingShader->setVec3("dirLight.direction", { 0.5f, 0.5f, 0.3f });
    lightingShader->setVec3("dirLight.ambient", { 0.2f, 0.2f, 0.2f });
    lightingShader->setVec3("dirLight.diffuse", { 0,0,0 });
    //lightingShader->setVec3("dirLight.diffuse", glm::vec3((sin(timeGetTime() / 3000.0f) + 1) / 2));
    lightingShader->setVec3("dirLight.specular", { 0.0f, 0.0f, 0.0f });

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(camera->GetFov()), (float)width / (float)height, 0.1f, 100.0f);
    glm::mat4 view = camera->GetViewMatrix();
    lightingShader->setMat4("projection", projection);
    lightingShader->setMat4("view", view);



    for (int i = 0; i < 30 * 30; i++)
    {
        tiles[i].Render(lightingShader);
    }

    player->Render(lightingShader);
    characterUI->Render(UIShader);
	glFlush();
}

PlayScene::~PlayScene()
{
}
