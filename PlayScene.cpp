#include "PlayScene.h"
#include "CharacterUI.h"
#include "Character.h"
#include "Shader.h"
#include "Camera.h"
#include "Tile.h"
#include "TileManager.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "BurnerMiningDrill.h"
#include "BurnerMiningDrillUI.h"
#include "EntityManager.h"
#include "Ore.h"
#include "GLImage.h"
#include "StructureBuilder.h"
#include "HandUI.h"

HRESULT PlayScene::Init()
{
    SetWindowSize((1920 - width) / 2, (1080 - height) / 2, width, height);
    glEnable(GL_TEXTURE_2D);
    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_ALWAYS);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD); // this is default
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    UIManager::GetSingleton()->Init();

    // player
    player = new Character();
    player->Init();

    tileRenderer = TileManager::GetSingleton();
    tileRenderer->Init();

    // camera
    camera = new Camera();
    camera->Init();
    camera->SetPosition({ 0.0f, 0.0f, 1.0f });
    camera->SetTarget(player->GetLpPosition());

    numOfPointLight = 4;

    //  Uniform Buffer Object 초기화
    glGenBuffers(1, &uboMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glGenBuffers(1, &uboLights);
    glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
    glBufferData(GL_UNIFORM_BUFFER, DirectionalLight::std140Size() + PointLight::std140Size() * numOfPointLight, NULL, GL_DYNAMIC_DRAW);
    glBindBufferRange(GL_UNIFORM_BUFFER, 1, uboLights, 0, DirectionalLight::std140Size() + PointLight::std140Size() * numOfPointLight * numOfPointLight);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glGenBuffers(1, &uboUIMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, uboUIMatrices);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, uboUIMatrices, 0, sizeof(glm::mat4));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// build and compile our shader zprogram
	// ------------------------------------
	lightingShader = new Shader("StandardVertexShader.glsl", "StandardFragmentShader.glsl");
    UIShader = new Shader("UIVertexShader.glsl", "UIFragmentShader.glsl");
    
    // UI Init
    CharacterUI* characterUI = new CharacterUI();
    characterUI->Init(player->GetLpInventory());
    characterUI->SetLocalPosition(glm::vec2(width / 2, height / 2));
    UIManager::GetSingleton()->AddUI("CharacterUI", characterUI);

    UI* burnerMiningDrillUI = new BurnerMiningDrillUI();
    burnerMiningDrillUI->Init();
    burnerMiningDrillUI->SetLocalPosition(glm::vec2(width / 2, height / 2));
    UIManager::GetSingleton()->AddUI("BurnerMiningDrillUI", burnerMiningDrillUI);

	// shader configuration
	// --------------------
    UIShader->use();
    UIShader->setInt("material.diffuse", 0);

	lightingShader->use();
    lightingShader->setInt("material.diffuse", 0);
    lightingShader->setVec3("material.diffuseColor", vec3(1.0f, 1.0f, 1.0f));
    glBindBuffer(GL_UNIFORM_BUFFER, uboLights);

    // directional light(static)
    dirLight = new DirectionalLight();
    dirLight->ambient = { 0.01f, 0.01f, 0.01f };
    dirLight->diffuse = { 0.3f, 0.3f, 0.3f };
    glBufferSubData(GL_UNIFORM_BUFFER,                  0, sizeof(glm::vec3), glm::value_ptr(dirLight->ambient));
    glBufferSubData(GL_UNIFORM_BUFFER,  sizeof(glm::vec4), sizeof(glm::vec3), glm::value_ptr(dirLight->diffuse));

    pointLights = new PointLight[numOfPointLight]();
    pointLights[0].position =   { 0.0f,   0.0f,  0.0f };
    pointLights[0].ambient =    { 0.01f, 0.01f, 0.01f };
    pointLights[0].diffuse =    { 0.6f,   0.6f,  0.6f };
    pointLights[0].constant =   0.09f;
    pointLights[0].linear =     0.0008f;
    pointLights[0].quadratic =  0.000352f;
    
    pointLights[1].position = { 1000.0f, 0.0f, 0.0f };
    pointLights[1].ambient = { 0.01f, 0.01f, 0.01f };
    pointLights[1].diffuse = { 0.8f, 0.8f, 0.8f };
    pointLights[1].constant = 2.0f;
    pointLights[1].linear = 0.003f;
    pointLights[1].quadratic = 0.022f;

    pointLights[2].position = { 0.0f,  1000.0f, 0.0f };
    pointLights[2].ambient = { 0.01f, 0.01f, 0.01f };
    pointLights[2].diffuse = { 0.8f, 0.8f, 0.8f };
    pointLights[2].constant = 3.0f;
    pointLights[2].linear = 0.002f;
    pointLights[2].quadratic = 0.012f;

    pointLights[3].position = { 1000.0f,  1000.0f, 0.0f };
    pointLights[3].ambient = { 0.01f, 0.01f, 0.01f };
    pointLights[3].diffuse = { 0.8f, 0.8f, 0.8f };
    pointLights[3].constant = 2.0f;
    pointLights[3].linear = 0.0001f;
    pointLights[3].quadratic = 0.002f;

    for (int i = 0; i < numOfPointLight; i++)
    {
        glBufferSubData(GL_UNIFORM_BUFFER,                                                PointLight::std140Size() * i + DirectionalLight::std140Size(), sizeof(glm::vec3), glm::value_ptr(pointLights[i].position));
        glBufferSubData(GL_UNIFORM_BUFFER,                        1 * sizeof(glm::vec4) + PointLight::std140Size() * i + DirectionalLight::std140Size(), sizeof(glm::vec3), glm::value_ptr(pointLights[i].ambient));
        glBufferSubData(GL_UNIFORM_BUFFER,                        2 * sizeof(glm::vec4) + PointLight::std140Size() * i + DirectionalLight::std140Size(), sizeof(glm::vec3), glm::value_ptr(pointLights[i].diffuse));
        glBufferSubData(GL_UNIFORM_BUFFER,                        3 * sizeof(glm::vec4) + PointLight::std140Size() * i + DirectionalLight::std140Size(), sizeof(glm::float_t), &pointLights[i].constant);
        glBufferSubData(GL_UNIFORM_BUFFER,  1 * sizeof(float_t) + 3 * sizeof(glm::vec4) + PointLight::std140Size() * i + DirectionalLight::std140Size(), sizeof(glm::float_t), &pointLights[i].linear);
        glBufferSubData(GL_UNIFORM_BUFFER,  2 * sizeof(float_t) + 3 * sizeof(glm::vec4) + PointLight::std140Size() * i + DirectionalLight::std140Size(), sizeof(glm::float_t), &pointLights[i].quadratic);
    }
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // TextRenderer Init
    textRenderer = TextRenderer::GetSingleton();
    textRenderer->Init();
    textRenderer->Load("Fonts/NotoSans-Bold.ttf", 24);

    entityManager = new EntityManager();
    entityManager->Init();
    
    BurnerMiningDrill* drill;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {   
            drill = new BurnerMiningDrill();
            drill->Init(j * 128, i * 128);
            entityManager->AddEntity(drill);
        }
    }

    structureBuilder = new StructureBuilder();
    structureBuilder->Init(entityManager);
	return S_OK;
}

void PlayScene::Release()
{
    SAFE_RELEASE(structureBuilder);
    SAFE_RELEASE(entityManager);
    SAFE_RELEASE(textRenderer);
    if (tileRenderer) 
    {
        tileRenderer->Release();
        tileRenderer = nullptr;
    }

    SAFE_RELEASE(player);
    SAFE_DELETE(UIShader);
	SAFE_DELETE(lightingShader);
    SAFE_RELEASE(camera);

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
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
        if (UIManager::GetSingleton()->IsCurrUINull())
            UIManager::GetSingleton()->ActiveUI("CharacterUI");
        else
            UIManager::GetSingleton()->DeactiveUI();
    }

    UIManager::GetSingleton()->Update();

    entityManager->Update();

    tileRenderer->Update();

    player->Update();
    
    for (int i = 0; i < entityManager->GetSize(); i++)
    {
        FRECT colRect;
        FRECT playerRect = player->GetCollisionFRect();
        FRECT entityRect = entityManager->GetEntity(i)->GetCollisionFRect();
        if (IntersectFRect(&colRect, &playerRect, &entityRect))
        {
            if (abs(colRect.right - colRect.left) < abs(colRect.top - colRect.bottom))
            {
                // 우측 충돌일때
                if (colRect.right == entityRect.right)
                    player->GetLpPosition()->x += abs(colRect.right - colRect.left);
                else
                    player->GetLpPosition()->x -= abs(colRect.right - colRect.left);
            }
            else
            {
                // 아래측 충돌일 때
                if (colRect.bottom == entityRect.bottom)
                    player->GetLpPosition()->y -= abs(colRect.top - colRect.bottom);
                else
                    player->GetLpPosition()->y += abs(colRect.top - colRect.bottom);
            }
        }
    }

    g_cursorPosition = { (g_ptMouse.x - width / 2) / camera->GetZoom() + camera->GetPosition().x,
                    (g_ptMouse.y - height / 2) / camera->GetZoom() + camera->GetPosition().y };
    g_cursorCoord = POS_TO_COORD(g_cursorPosition);

    if (!UIManager::GetSingleton()->IsMouseOnUI())
    {
        ItemInfo* info = UIManager::GetSingleton()->GetLpHandUI()->GetLpSelectedSlot();
        if (info && g_itemSpecs[info->id].buildable)
            structureBuilder->Active();
        else
            structureBuilder->Deactive();
    }
    else
        structureBuilder->Deactive();

    
    structureBuilder->Update(player->GetLpPosition());
	camera->Update();
    
}

void PlayScene::Render(HDC hdc)
{
	glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // be sure to activate shader when setting uniforms/drawing objects
    lightingShader->use();
    
    glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
    // player point light(dynamic)
    pointLights[0].position = glm::vec3(player->GetLpPosition()->x, player->GetLpPosition()->y, 0.01f);
    glBufferSubData(GL_UNIFORM_BUFFER, DirectionalLight::std140Size(), sizeof(glm::vec3), glm::value_ptr(pointLights[0].position));
    // directional light(dynamic)
    dirLight->diffuse = glm::vec3((sin(timeGetTime() / 3000.0f) + 1));
    //dirLight->diffuse = glm::vec3(0.0f);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(glm::vec3), glm::value_ptr(dirLight->diffuse));

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
    // 뷰/프로젝션 매트릭스 연산
    glm::mat4 projection = glm::ortho(0.0f, float(width) / camera->GetZoom(), 0.0f, float(height) / camera->GetZoom());
    glm::mat4 view = camera->GetViewMatrix();
    view = glm::translate(view, glm::vec3(float(width) / camera->GetZoom() / 2, float(height) / camera->GetZoom() / 2, 0.0f));

    // uniform buffer object 0번 바인딩 인덱스에 프로젝션과 뷰 매트릭스 설정
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER,                  0, sizeof(glm::mat4), glm::value_ptr(projection));
    glBufferSubData(GL_UNIFORM_BUFFER,  sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // UI프로젝션 매트릭스 연산
    glm::mat4 UIprojection = glm::ortho(0.0f, float(width), 0.0f, float(height));

    // uniform buffer object 2번 바인딩 인덱스에 UI프로젝션 설정
    glBindBuffer(GL_UNIFORM_BUFFER, uboUIMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER,                  0, sizeof(glm::mat4), glm::value_ptr(UIprojection));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);


    tileRenderer->Render(camera->GetRect(width, height));
    float tileRenderer_time = TimerManager::GetSingleton()->CheckTime();

    entityManager->Render(lightingShader);
    float entityManager_time = TimerManager::GetSingleton()->CheckTime();

    player->Render(lightingShader);
    float player_time = TimerManager::GetSingleton()->CheckTime();

    structureBuilder->Render(lightingShader);
    float structureBuilder_time = TimerManager::GetSingleton()->CheckTime();

    UIManager::GetSingleton()->Render(UIShader);
    float UIManager_time = TimerManager::GetSingleton()->CheckTime();

   
    char str[128];

    textRenderer->RenderText("FPS: " + to_string(TimerManager::GetSingleton()->GetFPS()),
        10, height - 10);

    textRenderer->RenderText("g_ptMouse : " + to_string(g_ptMouse.x) + ", " + to_string(g_ptMouse.y), 
        10, height - 40);

    sprintf_s(str, "cameraPos: (%.1f, %.1f)", camera->GetPosition().x, camera->GetPosition().y);
    textRenderer->RenderText(string(str),
        10, height - 70);

    sprintf_s(str, "g_cursorPosition: (%.1f, %.1f)", g_cursorPosition.x, g_cursorPosition.y );
    textRenderer->RenderText(string(str),
        10, height - 100);

    sprintf_s(str, "Cursor: (%d, %d)", (int)g_cursorCoord.x, (int)g_cursorCoord.y);
    textRenderer->RenderText(string(str),
        10, height - 130);

    Tile* tile = TileManager::GetSingleton()->GetLPTileUnderMouse();
    if (tile)
    {
        sprintf_s(str, "Ore Amount: (%d)", tile->GetLpOre()->GetAmount() );
        textRenderer->RenderText(string(str),
            10, height - 160);
        sprintf_s(str, "StructureAddress: (%p)", tile->GetLpSturcture());
        textRenderer->RenderText(string(str),
            10, height - 190);
    }

    textRenderer->RenderText("Zoom: " + to_string(camera->GetZoom()),
        10, height - 220);

    textRenderer->RenderText("UpdateTime: " + to_string(TimerManager::GetSingleton()->updateTime),
        10, height - 250);

    textRenderer->RenderText("tileRenderer_time:         " + to_string(tileRenderer_time),
        10, height - 280);                           
    textRenderer->RenderText("entityManager_time:      " + to_string(entityManager_time),
        10, height - 310);                           
    textRenderer->RenderText("player_time:           " + to_string(player_time),
        10, height - 340);
    textRenderer->RenderText("structureBuilder_time: " + to_string(structureBuilder_time),
        10, height - 370);
    textRenderer->RenderText("UIManager_time:          " + to_string(UIManager_time),
        10, height - 400);
    textRenderer->RenderText("TextRender_time:         " + to_string(TimerManager::GetSingleton()->CheckTime()),
        10, height - 430);
    textRenderer->RenderText("RenderTime:              " + to_string(TimerManager::GetSingleton()->renderTime),
        10, height - 460);

	glFlush();
}

PlayScene::~PlayScene()
{
}
