#include "GLImage.h"
#include "Texture.h"
#include "Shader.h"
#include "Scene.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"

HRESULT GLImage::Init(char const* sourceTexture, int maxFrameX, int maxFrameY, int width, int height)
{
	this->sourceTexture = TextureManager::GetSingleton()->FindTexture(sourceTexture);
    if (sourceTexture == nullptr)
        return E_FAIL;

    // Load base width, height;
    if (width == -1) 
        width = this->sourceTexture->GetWidth();
    if (height == -1) 
        height = this->sourceTexture->GetHeight();

	maxFrame.x = maxFrameX;
	maxFrame.y = maxFrameY;
    frameWidth = width / maxFrame.x;
    frameHeight = height / maxFrame.y;
    
    zoom = 1.0f;
    
    float vertices[] = {
        // positions                        // texture coords
        -frameWidth / 2, -frameHeight / 2,  0.0f, 0.0f,
         frameWidth / 2, -frameHeight / 2,  1.0f, 0.0f,
         frameWidth / 2,  frameHeight / 2,  1.0f, 1.0f,
         frameWidth / 2,  frameHeight / 2,  1.0f, 1.0f,
        -frameWidth / 2,  frameHeight / 2,  0.0f, 1.0f,
        -frameWidth / 2, -frameHeight / 2,  0.0f, 0.0f,
    };

    imageVAO = new VAO();
    VBO* posTexcoordVBO = new VBO();
    posTexcoordVBO->SetData(sizeof(vertices), vertices, GL_STATIC_DRAW);
    imageVAO->AddVBO(0, posTexcoordVBO, 4);

    isInit = true;
	return S_OK;
}

void GLImage::Release()
{
    if (isInit)
    {
        SAFE_DELETE(imageVAO);
    }
}

void GLImage::Render(Shader* shader, float destX, float destY, int currFrameX, int currFrameY)
{
    if (sourceTexture == nullptr)
        return;

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
    glBindVertexArray(imageVAO->GetID());
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glBindVertexArray(0);
}

void GLImage::AnimationRender(Shader* shader, float destX, float destY, Animation* ani)
{
}
