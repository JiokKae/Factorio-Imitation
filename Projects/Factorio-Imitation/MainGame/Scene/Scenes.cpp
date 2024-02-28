#include "Scenes.h"

#include "GLFramework/Camera/Camera.h"
#include "GLFramework/Camera3D/FreeCamera.h"
#include "GLFramework/Light/DirectionalLight.h"
#include "GLFramework/Light/PointLight.h"
#include "GLFramework/ShaderProgram/ShaderProgram.h"
#include "GLFramework/Texture/Texture.h"
#include "GLFramework/UI/UI.h"
#include "2DFramework/Button/Button.h"
#include "2DFramework/Image/Image.h"
#include "../Button/ButtonFunction.h"
#include "../../Character.h"
#include "../../EntityManager.h"
#include "../../MainGameUIFactory.h"
#include "../../Ore.h"
#include "../../Structure.h"
#include "../../Tile.h"
#include "../../TileManager.h"

HRESULT LightingScene::Init()
{
	SetWindowSize((1920 - width) / 2, (1080 - height) / 2, width, height);
	glDepthFunc(GL_LESS);

	// camera
	camera = new FreeCamera();

	camera->Init(glm::ivec2(WINSIZE_X, WINSIZE_Y));

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader zprogram
	// ------------------------------------
	lightingShader = new ShaderProgram("shader/1.colors.vert", "shader/1.colors.frag");
	lightCubeShader = new ShaderProgram("shader/1.light_cube.vert", "shader/1.light_cube.frag");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};
	// first, configure the cube's VAO (and VBO)

	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texcoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);

	// we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 텍스처 로드 및 생성
	diffuseMap = new Texture("base/graphics/entity/wooden-chest/hr-wooden-chest.png");
	specularMap = new Texture("base/graphics/entity/wooden-chest/hr-wooden-chest_specular.png");

	// shader configuration
	// --------------------
	lightingShader->use();
	lightingShader->setInt("material.diffuse", diffuseMap->GetID());
	lightingShader->setInt("material.specular", specularMap->GetID());

	return S_OK;
}

void LightingScene::Release()
{
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightCubeVAO);
	glDeleteBuffers(1, &VBO);

	SAFE_DELETE(specularMap);
	SAFE_DELETE(diffuseMap);
	SAFE_DELETE(lightCubeShader);
	SAFE_DELETE(lightingShader);
	SAFE_RELEASE(camera);
}

void LightingScene::Update()
{
	if (KeyManager::GetSingleton()->IsStayKeyDown(VK_ESCAPE))
	{
		SceneManager::GetSingleton()->ChangeScene("TitleScene");
		return;
	}

	camera->Update(g_mousezDelta, TimerManager::GetSingleton()->GetTimeElapsed(), glm::ivec2(g_ptMouse.x, g_ptMouse.y));
}

void LightingScene::Render(HDC /*hdc*/)
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::vec3 cubePositions[] = {
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
	glm::vec3 pointLightPositions[] = {
	    glm::vec3(1.7f,  1.2f,  2.0f),
	    glm::vec3(2.3f, -3.3f, -4.0f),
	    glm::vec3(-4.0f,  2.0f, -12.0f),
	    glm::vec3(1.0f,  2.0f, -3.0f)
	};

	// be sure to activate shader when setting uniforms/drawing objects
	lightingShader->use();

	// change the light's position values over time (can be done anywhere in the render loop actually, but try to do it at least before using the light source positions)
	lightingShader->setVec3("viewPos", camera->GetPosition());
	lightingShader->setFloat("material.shininess", 64.0f);

	/*
	      Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
	      the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
	      by defining light types as classes and set their values in there, or by using a more efficient uniform approach
	      by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
	*/
	// directional light
	lightingShader->setVec3("dirLight.direction", { -0.2f, -1.0f, -0.3f });
	lightingShader->setVec3("dirLight.ambient", { 0.05f, 0.05f, 0.05f });
	lightingShader->setVec3("dirLight.diffuse", { 0.01f, 0.01f, 0.01f });
	lightingShader->setVec3("dirLight.specular", { 0.0f, 0.0f, 0.0f });
	// point light 1
	lightingShader->setVec3("pointLights[0].position", pointLightPositions[0]);
	lightingShader->setVec3("pointLights[0].ambient", { 0.05f, 0.05f, 0.05f });
	lightingShader->setVec3("pointLights[0].diffuse", { 0.8f, 0.8f, 0.8f });
	lightingShader->setVec3("pointLights[0].specular", { 0, 0, 0 });
	lightingShader->setFloat("pointLights[0].constant", 1.0f);
	lightingShader->setFloat("pointLights[0].linear", 0.09f);
	lightingShader->setFloat("pointLights[0].quadratic", 0.032f);
	// point light 2
	lightingShader->setVec3("pointLights[1].position", pointLightPositions[1]);
	lightingShader->setVec3("pointLights[1].ambient", { 0.05f, 0.05f, 0.05f });
	lightingShader->setVec3("pointLights[1].diffuse", { 0.8f, 0.8f, 0.8f });
	lightingShader->setVec3("pointLights[1].specular", { 1.0f, 1.0f, 1.0f });
	lightingShader->setFloat("pointLights[1].constant", 1.0f);
	lightingShader->setFloat("pointLights[1].linear", 0.09f);
	lightingShader->setFloat("pointLights[1].quadratic", 0.032f);
	// point light 3
	lightingShader->setVec3("pointLights[2].position", pointLightPositions[2]);
	lightingShader->setVec3("pointLights[2].ambient", { 0.05f, 0.05f, 0.05f });
	lightingShader->setVec3("pointLights[2].diffuse", { 0.8f, 0.8f, 0.8f });
	lightingShader->setVec3("pointLights[2].specular", { 1.0f, 1.0f, 1.0f });
	lightingShader->setFloat("pointLights[2].constant", 1.0f);
	lightingShader->setFloat("pointLights[2].linear", 0.09f);
	lightingShader->setFloat("pointLights[2].quadratic", 0.032f);
	// point light 4
	lightingShader->setVec3("pointLights[3].position", pointLightPositions[3]);
	lightingShader->setVec3("pointLights[3].ambient", { 0.05f, 0.05f, 0.05f });
	lightingShader->setVec3("pointLights[3].diffuse", { 0.8f, 0.8f, 0.8f });
	lightingShader->setVec3("pointLights[3].specular", { 1.0f, 1.0f, 1.0f });
	lightingShader->setFloat("pointLights[3].constant", 1.0f);
	lightingShader->setFloat("pointLights[3].linear", 0.09f);
	lightingShader->setFloat("pointLights[3].quadratic", 0.032f);
	// spotLight
	lightingShader->setVec3("spotLight.position", camera->GetPosition());
	lightingShader->setVec3("spotLight.direction", camera->GerFront());
	lightingShader->setVec3("spotLight.ambient", { 0.0f, 0.0f, 0.0f });
	lightingShader->setVec3("spotLight.diffuse", { 1.0f, 1.0f, 1.0f });
	lightingShader->setVec3("spotLight.specular", { 1.0f, 1.0f, 1.0f });
	lightingShader->setFloat("spotLight.constant", 1.0f);
	lightingShader->setFloat("spotLight.linear", 0.09f);
	lightingShader->setFloat("spotLight.quadratic", 0.032f);
	lightingShader->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	lightingShader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
	/*
	lightingShader->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	lightingShader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
	*/

	// view/projection transformations
	glm::mat4 projection = glm::perspective(glm::radians(camera->GetFov()), (float)width / (float)height, 0.1f, 100.0f);
	glm::mat4 view = camera->GetViewMatrix();
	lightingShader->setMat4("projection", projection);
	lightingShader->setMat4("view", view);

	// world transformation
	lightingShader->setMat4("model", glm::mat4(1.0f));

	// bind diffuse map
	glActiveTexture(GL_TEXTURE0 + diffuseMap->GetID());
	glBindTexture(GL_TEXTURE_2D, diffuseMap->GetID());
	// bind specular map
	glActiveTexture(GL_TEXTURE0 + specularMap->GetID());
	glBindTexture(GL_TEXTURE_2D, specularMap->GetID());

	// render the cube
	glBindVertexArray(cubeVAO);
	for (unsigned int i = 0; i < 10; i++)
	{
		glm::mat4 model;
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i;
		model = glm::rotate(model, glm::radians(angle) * (float)timeGetTime() / 1000.0f, glm::vec3(1.0f, 0.3f, 0.5f));
		lightingShader->setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	// also draw the lamp object
	lightCubeShader->use();
	lightCubeShader->setMat4("projection", projection);
	lightCubeShader->setMat4("view", view);


	glBindVertexArray(lightCubeVAO);
	for (unsigned int i = 0; i < 4; i++)
	{
		glm::mat4 model;
		model = glm::mat4(1.0f);
		model = glm::translate(model, pointLightPositions[i]);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		lightCubeShader->setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	glFlush();
}

LightingScene::~LightingScene()
{
}

HRESULT LoadingScene1::Init()
{
	this->SetUseBackBuffer(true);

	img = ImageManager::GetSingleton()->FindImage("Loading Scene Image");

	return S_OK;
}

void LoadingScene1::Release()
{
}

void LoadingScene1::Update()
{
}

void LoadingScene1::Render(HDC hdc)
{
	if (img)
		img->Render(hdc, 0, 0, width, height);
}

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

	entityManager = EntityManager::GetSingleton();
	entityManager->Init();

	tileManager = TileManager::GetSingleton();
	tileManager->Init();

	// camera
	camera = new Camera();
	camera->SetPosition({ 0.0f, 0.0f, 1.0f });
	camera->SetFollowTarget(entityManager->GetLpPlayer()->GetLpPosition());

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
	lightingShader = new ShaderProgram("shader/StandardVertexShader.glsl", "shader/StandardFragmentShader.glsl");
	UIShader = new ShaderProgram("shader/UIVertexShader.glsl", "shader/UIFragmentShader.glsl");

	// UI Init
	UI* characterUI = MainGameUIFactory::CreateUI(MainGameUIFactory::CHARACTER_UI);
	characterUI->Init();
	characterUI->SetLocalPosition(glm::vec2(width / 2, height / 2));
	UIManager::GetSingleton()->AddUI("CharacterUI", characterUI);

	UI* burnerMiningDrillUI = MainGameUIFactory::CreateUI(MainGameUIFactory::BURNER_MINING_DRILL_UI);
	burnerMiningDrillUI->Init();
	burnerMiningDrillUI->SetLocalPosition(glm::vec2(width / 2, height / 2));
	UIManager::GetSingleton()->AddUI("BurnerMiningDrillUI", burnerMiningDrillUI);

	UI* stoneFurnaceUI = MainGameUIFactory::CreateUI(MainGameUIFactory::STONE_FURNACE_UI);
	stoneFurnaceUI->Init();
	stoneFurnaceUI->SetLocalPosition(glm::vec2(width / 2, height / 2));
	UIManager::GetSingleton()->AddUI("StoneFurnaceUI", stoneFurnaceUI);

	UI* selectRecipeAssemblingUI = MainGameUIFactory::CreateUI(MainGameUIFactory::SELECT_RECIPE_ASSEMBLING_UI);
	selectRecipeAssemblingUI->Init();
	selectRecipeAssemblingUI->SetLocalPosition(glm::vec2(width / 2, height / 2));
	UIManager::GetSingleton()->AddUI("SelectRecipeAssemblingUI", selectRecipeAssemblingUI);

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
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec3), glm::value_ptr(dirLight->ambient));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(glm::vec3), glm::value_ptr(dirLight->diffuse));

	pointLights = new PointLight[numOfPointLight]();
	pointLights[0].position = { 0.0f,   0.0f,  0.0f };
	pointLights[0].ambient = { 0.01f, 0.01f, 0.01f };
	pointLights[0].diffuse = { 0.6f,   0.6f,  0.6f };
	pointLights[0].constant = 0.09f;
	pointLights[0].linear = 0.0008f;
	pointLights[0].quadratic = 0.000352f;

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
		glBufferSubData(GL_UNIFORM_BUFFER, PointLight::std140Size() * i + DirectionalLight::std140Size(), sizeof(glm::vec3), glm::value_ptr(pointLights[i].position));
		glBufferSubData(GL_UNIFORM_BUFFER, 1 * sizeof(glm::vec4) + PointLight::std140Size() * i + DirectionalLight::std140Size(), sizeof(glm::vec3), glm::value_ptr(pointLights[i].ambient));
		glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec4) + PointLight::std140Size() * i + DirectionalLight::std140Size(), sizeof(glm::vec3), glm::value_ptr(pointLights[i].diffuse));
		glBufferSubData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::vec4) + PointLight::std140Size() * i + DirectionalLight::std140Size(), sizeof(glm::float_t), &pointLights[i].constant);
		glBufferSubData(GL_UNIFORM_BUFFER, 1 * sizeof(float_t) + 3 * sizeof(glm::vec4) + PointLight::std140Size() * i + DirectionalLight::std140Size(), sizeof(glm::float_t), &pointLights[i].linear);
		glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(float_t) + 3 * sizeof(glm::vec4) + PointLight::std140Size() * i + DirectionalLight::std140Size(), sizeof(glm::float_t), &pointLights[i].quadratic);
	}
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	textRenderer = TextRenderer::GetSingleton();

	debugHelper = DebugHelper::GetSingleton();

	return S_OK;
}

void PlayScene::Release()
{
	if (entityManager)
	{
		entityManager->Release();
		entityManager = nullptr;
	}
	if (tileManager)
	{
		tileManager->Release();
		tileManager = nullptr;
	}

	SAFE_DELETE(UIShader);
	SAFE_DELETE(lightingShader);
	SAFE_DELETE(camera);

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
	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_MENU))
	{
		altMode = !altMode;
	}

	UIManager::GetSingleton()->Update();

	entityManager->Update(camera->GetFRect(width, height));

	tileManager->Update();

	g_cursorPosition = { (g_ptMouse.x - width / 2) / camera->GetZoom() + camera->GetPosition().x,
			(g_ptMouse.y - height / 2) / camera->GetZoom() + camera->GetPosition().y };
	g_cursorCoord = ivec2(POS_TO_COORD(g_cursorPosition));

	g_currScreenSize = vec2(width, height);
	camera->Update(g_mousezDelta);

	debugHelper->SetFloat("1. Zoom", camera->GetZoom());
}

void PlayScene::Render(HDC /*hdc*/)
{
	glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// be sure to activate shader when setting uniforms/drawing objects
	lightingShader->use();

	glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
	// player point light(dynamic)
	pointLights[0].position = glm::vec3(entityManager->GetLpPlayer()->GetLpPosition()->x, entityManager->GetLpPlayer()->GetLpPosition()->y, 0.01f);
	glBufferSubData(GL_UNIFORM_BUFFER, DirectionalLight::std140Size(), sizeof(glm::vec3), glm::value_ptr(pointLights[0].position));
	// directional light(dynamic)
	dirLight->diffuse = glm::vec3((sin(g_time / 3000.0f) + 1));
	//dirLight->diffuse = glm::vec3(0.0f);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(glm::vec3), glm::value_ptr(dirLight->diffuse));

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// 뷰/프로젝션 매트릭스 연산
	glm::mat4 projection = camera->GetProjectionMatrix(width, height);
	glm::mat4 view = camera->GetViewMatrix();

	// uniform buffer object 0번 바인딩 인덱스에 프로젝션과 뷰 매트릭스 설정
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// UI프로젝션 매트릭스 연산
	glm::mat4 UIprojection = glm::ortho(0.0f, float(width), 0.0f, float(height));

	// uniform buffer object 2번 바인딩 인덱스에 UI프로젝션 설정
	glBindBuffer(GL_UNIFORM_BUFFER, uboUIMatrices);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(UIprojection));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	TimerManager* timerManager(TimerManager::GetSingleton());
	debugHelper->SetFloat("2. Render UpdateTime", timerManager->updateTime);

	tileManager->Render(camera->GetRect(width, height));
	debugHelper->SetFloat("3. tileRenderer_time", timerManager->CheckTime());

	entityManager->Render(lightingShader);
	debugHelper->SetFloat("4. entityManager_time", timerManager->CheckTime());

	UIManager::GetSingleton()->Render(UIShader);
	debugHelper->SetFloat("5. UIManager_time", timerManager->CheckTime());

	char str[128];
	textRenderer->RenderText("FPS: " + to_string(timerManager->GetFPS()), (float)10, (float)height - 10);

	if (g_debuggingMode)
	{
		textRenderer->RenderText("g_ptMouse : " + to_string(g_ptMouse.x) + ", " + to_string(g_ptMouse.y), 10.0f, static_cast<float>(height) - 40.0f);
		sprintf_s(str, "cameraPos: (%.1f, %.1f)", camera->GetPosition().x, camera->GetPosition().y);
		textRenderer->RenderText(string(str), 10.0f, height - 70.0f);
		sprintf_s(str, "g_cursorPosition: (%.1f, %.1f)", g_cursorPosition.x, g_cursorPosition.y);
		textRenderer->RenderText(string(str), 10.0f, height - 100.0f);
		sprintf_s(str, "Cursor: (%d, %d)", g_cursorCoord.x, g_cursorCoord.y);
		textRenderer->RenderText(string(str), 10.0f, height - 130.0f);
		Tile* tile = TileManager::GetSingleton()->GetLpCursorTile();
		if (tile)
		{
			sprintf_s(str, "Ore Amount: %d", tile->GetLpOre()->GetAmount());
			textRenderer->RenderText(string(str), 10.0f, height - 160.0f);
			sprintf_s(str, "Item Amount: %d", (int)tile->GetItems().size());
			textRenderer->RenderText(string(str), 10.0f, height - 190.0f);
		}
	}

	DebugHelper::GetSingleton()->Render(10.0f, height - 220.0f);

	glFlush();
}

PlayScene::~PlayScene()
{
}

HRESULT TenCubeSpaceScene::Init()
{
	SetWindowSize((1920 - width) / 2, (1080 - height) / 2, width, height);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
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
	// 1. Vertex Array Object 바인딩
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	// 2. OpenGL이 사용하기 위해 vertex 리스트를 vertex 버퍼에 복사
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 위치 attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 텍스쳐 attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// 5. 오브젝트를 그리고 싶을 때 우리가 생성한 shader program을 사용
	ourShader = new ShaderProgram("shader/SimpleVertexShader.glsl", "shader/SimpleFragmentShader.glsl");

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// 텍스처 로드 및 생성
	texture1 = new Texture("base/graphics/entity/assembling-machine-1/assembling-machine-1.png");
	texture2 = new Texture("base/graphics/entity/artillery-turret/hr-artillery-turret-base.png");

	ourShader->use(); // uniform을 설정하기 전에 shader를 활성화해야 한다는 것을 잊지마세요!  
	ourShader->setInt("texture1", texture1->GetID());
	ourShader->setInt("texture2", texture2->GetID());
	ourShader->setFloat("radio", radio);

	camera = new FreeCamera();
	camera->Init(glm::ivec2(WINSIZE_X, WINSIZE_Y));

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

	SAFE_DELETE(texture1);
	SAFE_DELETE(texture2);
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
	camera->Update(g_mousezDelta, TimerManager::GetSingleton()->GetTimeElapsed(), glm::ivec2(g_ptMouse.x, g_ptMouse.y));
	glm::mat4 view;
	view = camera->GetViewMatrix();

	ourShader->setMat4("view", view);
	ourShader->setMat4("projection", projection);
}

void TenCubeSpaceScene::Render(HDC /*hdc*/)
{
	// ..:: 드로잉 코드 (렌더링 루프 내부) :: ..

	// colorbuffer 비우기
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0 + texture1->GetID());
	glBindTexture(GL_TEXTURE_2D, texture1->GetID());
	glActiveTexture(GL_TEXTURE0 + texture2->GetID());
	glBindTexture(GL_TEXTURE_2D, texture2->GetID());

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

TenCubeSpaceScene::~TenCubeSpaceScene()
{
}

HRESULT TileMapToolScene::Init()
{
	SetWindowSize(50, 50, WINSIZE_TILE_MAP_X, WINSIZE_TILE_MAP_Y);
	this->SetUseBackBuffer(true);

	// 이미지 등록
	sampleTile = ImageManager::GetSingleton()->FindImage("SampleTile");
	selectTileEdge = ImageManager::GetSingleton()->FindImage("SelectTileEdge");

	// 세이브, 로드 버튼
	saveButton = new Button();
	saveButton->Init("SaveLoadButton", WINSIZE_TILE_MAP_X - sampleTile->GetWidth(), sampleTile->GetHeight() + 400, { 0, 0 }, { 0, 1 });
	saveButton->SetButtonFunc(ButtonFunction::TileInfoSave, Argument_Kind::TileInfoArgument, new TileInfoArgument(tileInfo));

	loadButton = new Button();
	loadButton->Init("SaveLoadButton", WINSIZE_TILE_MAP_X - sampleTile->GetWidth() + 200, sampleTile->GetHeight() + 400, { 1, 0 }, { 1, 1 });
	loadButton->SetButtonFunc(ButtonFunction::TileInfoLoad, Argument_Kind::TileInfoArgument, new TileInfoArgument(tileInfo));

	// 셀렉트 영역 초기화
	selectTileInfo.frame.left = 0;
	selectTileInfo.frame.right = 0;
	selectTileInfo.frame.top = 0;
	selectTileInfo.frame.bottom = 0;

	// 오른쪽 상단에 샘플 타일의 정보를 세팅

	rcSample.left = WINSIZE_TILE_MAP_X - sampleTile->GetWidth() - 50;
	rcSample.right = WINSIZE_TILE_MAP_X - 50;
	rcSample.top = 50;
	rcSample.bottom = 50 + sampleTile->GetHeight();
	for (int i = 0; i < SAMPLE_TILE_Y; i++)
	{
		for (int j = 0; j < SAMPLE_TILE_X; j++)
		{
			SetRect(&sampleTileInfo[i * TILE_X + j].rc,
				WINSIZE_TILE_MAP_X - sampleTile->GetWidth() + j * TILE_SIZE,
				WINSIZE_TILE_MAP_X - sampleTile->GetWidth() + i * TILE_SIZE,
				WINSIZE_TILE_MAP_X - sampleTile->GetWidth() + j * TILE_SIZE + TILE_SIZE,
				WINSIZE_TILE_MAP_X - sampleTile->GetWidth() + i * TILE_SIZE + TILE_SIZE);

			sampleTileInfo[i * TILE_X + j].frameX = j;
			sampleTileInfo[i * TILE_X + j].frameY = i;
		}
	}

	// 왼쪽 상단에 메인 타일의 정보를 세팅
	rcMain.left = 0;
	rcMain.top = 0;
	rcMain.right = rcMain.left + TILE_X * TILE_SIZE;
	rcMain.bottom = rcMain.top + TILE_X * TILE_SIZE;
	for (int i = 0; i < TILE_Y; i++)
	{
		for (int j = 0; j < TILE_X; j++)
		{
			SetRect(&tileInfo[i * TILE_X + j].rc,
				j * TILE_SIZE,
				i * TILE_SIZE,
				j * TILE_SIZE + TILE_SIZE,
				i * TILE_SIZE + TILE_SIZE);

			tileInfo[i * TILE_X + j].terrian = TERRAIN::GRASS;
			tileInfo[i * TILE_X + j].frameX = 0;
			tileInfo[i * TILE_X + j].frameY = 1;
		}
	}

	Sleep(250);

	return S_OK;
}

void TileMapToolScene::Release()
{
}

void TileMapToolScene::Update()
{
	if (KeyManager::GetSingleton()->IsStayKeyDown(VK_ESCAPE))
	{
		SceneManager::GetSingleton()->ChangeScene("TitleScene", "LoadingScene1");
		return;
	}

#pragma region 선택 영역
	// 샘플 타일 영역 안 인지 확인
	if (sampleSelectStart)
	{
		g_ptMouse.x = Clamp(g_ptMouse.x, rcSample.left, rcSample.right);
		g_ptMouse.y = Clamp(g_ptMouse.y, rcSample.top, rcSample.bottom - 1);
	}

	if (PtInRect(&rcSample, g_ptMouse))
	{
		if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON))
		{
			int posX = g_ptMouse.x - rcSample.left;
			int posY = g_ptMouse.y - rcSample.top;
			int idX = posX / TILE_SIZE;
			int idY = posY / TILE_SIZE;

			selectTileInfo.frame.left = idX;
			selectTileInfo.frame.right = idX;
			selectTileInfo.frame.top = idY;
			selectTileInfo.frame.bottom = idY;

			selectStartFrame = { idX, idY };
			sampleSelectStart = true;
		}

		if (sampleSelectStart)
		{
			if (KeyManager::GetSingleton()->IsStayKeyDown(VK_LBUTTON))
			{
				int posX = g_ptMouse.x - rcSample.left;
				int posY = g_ptMouse.y - rcSample.top;
				int idX = posX / TILE_SIZE;
				int idY = posY / TILE_SIZE;

				if (selectStartFrame.x <= idX)
				{
					selectTileInfo.frame.left = selectStartFrame.x;
					selectTileInfo.frame.right = idX;
				}
				else
				{
					selectTileInfo.frame.right = selectStartFrame.x;
					selectTileInfo.frame.left = idX;
				}
				if (selectStartFrame.y <= idY)
				{
					selectTileInfo.frame.top = selectStartFrame.y;
					selectTileInfo.frame.bottom = idY;
				}
				else
				{
					selectTileInfo.frame.bottom = selectStartFrame.y;
					selectTileInfo.frame.top = idY;
				}
			}
		}

		if (KeyManager::GetSingleton()->IsOnceKeyUp(VK_LBUTTON))
		{
			sampleSelectStart = false;
		}
	}
#pragma endregion

#pragma region 메인 영역
	if (PtInRect(&rcMain, g_ptMouse))
	{
		if (KeyManager::GetSingleton()->IsOnceKeyUp(VK_LBUTTON))
		{
			int posX = g_ptMouse.x - rcMain.left;
			int posY = g_ptMouse.y - rcMain.top;
			int idX = posX / TILE_SIZE;
			int idY = posY / TILE_SIZE;

			for (int i = 0; i <= selectTileInfo.frame.bottom - selectTileInfo.frame.top; i++)
			{
				for (int j = 0; j <= selectTileInfo.frame.right - selectTileInfo.frame.left; j++)
				{
					//메인 영역 안에 있어야 복사
					if (idX + j > TILE_X - 1)
						continue;

					if ((idX + j) + (idY + i) * SAMPLE_TILE_X < TILE_X * TILE_Y)
					{
						tileInfo[(idX + j) + (idY + i) * SAMPLE_TILE_X].frameX = j + selectTileInfo.frame.left;
						tileInfo[(idX + j) + (idY + i) * SAMPLE_TILE_X].frameY = i + selectTileInfo.frame.top;
					}
				}
			}
		}
	}
#pragma endregion


	saveButton->Update(g_ptMouse.x, g_ptMouse.y, WINSIZE_Y);
	loadButton->Update(g_ptMouse.x, g_ptMouse.y, WINSIZE_Y);
}

void TileMapToolScene::Render(HDC hdc)
{
	PatBlt(hdc, 0, 0, WINSIZE_TILE_MAP_X, WINSIZE_TILE_MAP_Y, WHITENESS);

	if (sampleTile)
	{
		// 샘플 타일	
		sampleTile->Render(hdc, rcSample.left, rcSample.top, 640, 288);

		// 메인 타일
		for (int i = 0; i < TILE_X * TILE_Y; i++)
		{
			sampleTile->FrameRender(hdc, tileInfo[i].rc.left + (TILE_SIZE / 2), tileInfo[i].rc.top + (TILE_SIZE / 2), tileInfo[i].frameX, tileInfo[i].frameY);
		}

		// 선택된 타일들
		for (int i = 0; i <= selectTileInfo.frame.bottom - selectTileInfo.frame.top; i++)
		{
			for (int j = 0; j <= selectTileInfo.frame.right - selectTileInfo.frame.left; j++)
			{
				sampleTile->FrameRender(hdc,
					WINSIZE_TILE_MAP_X - sampleTile->GetWidth() + (j * TILE_SIZE),
					sampleTile->GetHeight() + 100 + (i * TILE_SIZE),
					j + selectTileInfo.frame.left, i + selectTileInfo.frame.top);
			}
		}
		selectTileEdge->FrameRender(hdc, rcSample.left + (selectTileInfo.frame.left * TILE_SIZE) + TILE_SIZE / 2,
			rcSample.top + (selectTileInfo.frame.top * TILE_SIZE) + TILE_SIZE / 2, 0, 0);
		selectTileEdge->FrameRender(hdc, rcSample.left + (selectTileInfo.frame.right * TILE_SIZE) + TILE_SIZE / 2,
			rcSample.top + (selectTileInfo.frame.top * TILE_SIZE) + TILE_SIZE / 2, 1, 0);
		selectTileEdge->FrameRender(hdc, rcSample.left + (selectTileInfo.frame.left * TILE_SIZE) + TILE_SIZE / 2,
			rcSample.top + (selectTileInfo.frame.bottom * TILE_SIZE) + TILE_SIZE / 2, 0, 1);
		selectTileEdge->FrameRender(hdc, rcSample.left + (selectTileInfo.frame.right * TILE_SIZE) + TILE_SIZE / 2,
			rcSample.top + (selectTileInfo.frame.bottom * TILE_SIZE) + TILE_SIZE / 2, 1, 1);
	}
	saveButton->Render(hdc);
	loadButton->Render(hdc);
}

HRESULT TitleScene::Init()
{
	this->SetUseBackBuffer(true);
	SetWindowSize((1920 - width) / 2, (1080 - height) / 2, width, height);

	img = ImageManager::GetSingleton()->FindImage("TitleSceneImage");
	mainMenuImage = ImageManager::GetSingleton()->FindImage("MainMenu");
	SoundManager::GetSingleton()->Play("Ambient/Expansion", 0.6f);

	gameStartButton = new Button();
	gameStartButton->Init("NewGameButton", width / 2, height - 500 + 43, { 0, 2 }, { 0, 1 });
	gameStartButton->SetButtonFunc(ButtonFunction::ChangeScene, Argument_Kind::ChangeSceneArgument, new ChangeSceneArgument("PlayScene", ""));

	button1 = new Button();
	button1->Init("NewGameButton", width / 2, height - 400, { 0, 2 }, { 0, 1 });
	button1->SetButtonFunc(ButtonFunction::ChangeScene, Argument_Kind::ChangeSceneArgument, new ChangeSceneArgument("TileMapToolScene", "LoadingScene1"));

	TenCubeButton = new Button();
	TenCubeButton->Init("NewGameButton", width / 2, height - 300, { 0, 2 }, { 0, 1 });
	TenCubeButton->SetButtonFunc(ButtonFunction::ChangeScene, Argument_Kind::ChangeSceneArgument, new ChangeSceneArgument("TenCubeSpaceScene", ""));

	lightingButton = new Button();
	lightingButton->Init("NewGameButton", width / 2, height - 200, { 0, 2 }, { 0, 1 });
	lightingButton->SetButtonFunc(ButtonFunction::ChangeScene, Argument_Kind::ChangeSceneArgument, new ChangeSceneArgument("LightingScene", ""));

	quitButton = new Button();
	quitButton->Init("ExitButton", width / 2 - 108, height - 100 - 304, { 0, 2 }, { 0, 1 });
	quitButton->SetButtonFunc(ButtonFunction::QuitProgram);

	Sleep(350);

	return S_OK;
}

void TitleScene::Release()
{
	SoundManager::GetSingleton()->Stop("Ambient/Expansion");

	SAFE_RELEASE(gameStartButton);
	SAFE_RELEASE(lightingButton);
	SAFE_RELEASE(button1);
	SAFE_RELEASE(TenCubeButton);
	SAFE_RELEASE(quitButton);
}

void TitleScene::Update()
{
	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_ESCAPE))
		SendMessage(g_hWnd, WM_DESTROY, 0, 0);

	if (gameStartButton)
		gameStartButton->Update(g_ptMouse.x, g_ptMouse.y, WINSIZE_Y);

	if (g_debuggingMode)
	{
		if (button1)
			button1->Update(g_ptMouse.x, g_ptMouse.y, WINSIZE_Y);
		if (TenCubeButton)
			TenCubeButton->Update(g_ptMouse.x, g_ptMouse.y, WINSIZE_Y);
		if (lightingButton)
			lightingButton->Update(g_ptMouse.x, g_ptMouse.y, WINSIZE_Y);
	}

	if (quitButton)
		quitButton->Update(g_ptMouse.x, g_ptMouse.y, WINSIZE_Y);
}

void TitleScene::Render(HDC hdc)
{
	if (img)
		img->Render(hdc, 0, 0, WINSIZE_TITLE_X, WINSIZE_TITLE_Y);

	if (mainMenuImage)
		mainMenuImage->FrameRender(hdc, WINSIZE_TITLE_X / 2, WINSIZE_TITLE_Y / 2, 0, 0);

	if (gameStartButton)
		gameStartButton->Render(hdc);

	if (g_debuggingMode)
	{
		if (button1)
			button1->Render(hdc);
		if (TenCubeButton)
			TenCubeButton->Render(hdc);
		if (lightingButton)
			lightingButton->Render(hdc);
	}

	if (quitButton)
		quitButton->Render(hdc);
}
