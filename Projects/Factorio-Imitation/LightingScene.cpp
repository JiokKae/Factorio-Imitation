#include "LightingScene.h"
#include "FreeCamera.h"
#include "Shader.h"
#include "Texture.h"

HRESULT LightingScene::Init()
{
    SetWindowSize((1920 - width) / 2, (1080 - height) / 2, width, height);
    glDepthFunc(GL_LESS);

	// camera
	camera = new FreeCamera();
	camera->Init();

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader zprogram
	// ------------------------------------
    lightingShader = new Shader("1.colors.vert", "1.colors.frag");
    lightCubeShader = new Shader("1.light_cube.vert", "1.light_cube.frag");

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
    diffuseMap = new Texture();
    diffuseMap->Init("base/graphics/entity/wooden-chest/hr-wooden-chest.png");
    specularMap = new Texture();
    specularMap->Init("base/graphics/entity/wooden-chest/hr-wooden-chest_specular.png");

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

    SAFE_RELEASE(specularMap);
    SAFE_RELEASE(diffuseMap);
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

    camera->Update();
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

