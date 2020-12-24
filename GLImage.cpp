#include "GLImage.h"
#include "Texture.h"
#include "Shader.h"
#include "Scene.h"

HRESULT GLImage::Init(char const* sourceTexture, int width, int height, int maxFrameX, int maxFrameY)
{
	this->sourceTexture = TextureManager::GetSingleton()->FindTexture(sourceTexture);
    if (sourceTexture == nullptr)
        return E_FAIL;

	maxFrame.x = maxFrameX;
	maxFrame.y = maxFrameY;
    frameWidth = width / maxFrame.x;
    frameHeight = height / maxFrame.y;
    
    zoom = 1.0f;
    
    float vertices[] = {
        // positions                               // texture coords
        -frameWidth / 2, -frameHeight / 2,  0.0f,  0.0f, 0.0f,
         frameWidth / 2, -frameHeight / 2,  0.0f,  1.0f, 0.0f,
         frameWidth / 2,  frameHeight / 2,  0.0f,  1.0f, 1.0f,
         frameWidth / 2,  frameHeight / 2,  0.0f,  1.0f, 1.0f,
        -frameWidth / 2,  frameHeight / 2,  0.0f,  0.0f, 1.0f,
        -frameWidth / 2, -frameHeight / 2,  0.0f,  0.0f, 0.0f,
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texcoord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    isInit = true;
	return S_OK;
}

void GLImage::Release()
{
    if (isInit)
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
}

void GLImage::Render(Shader* shader, float destX, float destY, int currFrameX, int currFrameY)
{
    shader->use();

    shader->setVec2("currFrame", { currFrameX, currFrameY });
    shader->setVec2("maxFrame", maxFrame);
    shader->setFloat("alpha", alpha);

    glm::vec3 position = glm::vec3(destX, destY, 0.0f);
    // world transformation
    glm::mat4 planeModel;
    planeModel = glm::translate(planeModel, position);
    planeModel = glm::scale(planeModel, glm::vec3(zoom));
    shader->setMat4("model", planeModel);

    // bind diffuse map
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sourceTexture->GetID());

    // render the cube
    glBindVertexArray(VAO);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glBindVertexArray(0);
}

void GLImage::AnimationRender(Shader* shader, float destX, float destY, Animation* ani)
{
}
