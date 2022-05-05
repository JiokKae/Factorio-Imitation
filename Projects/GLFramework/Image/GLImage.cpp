#include "GLImage.h"
#include "../Texture/Texture.h"
#include "../Texture/TextureManager.h"
#include "../Shader/Shader.h"
#include "../VertexDataObject/VertexArrayObject.h"
#include "../VertexDataObject/VertexBufferObject.h"

HRESULT GLImage::Init(char const* _sourceTexture, int maxFrameX, int maxFrameY, float marginX, float marginY, int width, int height)
{
	this->sourceTexture = TextureManager::GetSingleton()->FindTexture(_sourceTexture);
    if (_sourceTexture == nullptr)
        return E_FAIL;

    // Load base width, height;
    if (width == -1) 
        width = this->sourceTexture->GetWidth();
    if (height == -1) 
        height = this->sourceTexture->GetHeight();

	maxFrame.x = maxFrameX;
	maxFrame.y = maxFrameY;
    frameWidth = static_cast<float>(width / maxFrame.x);
    frameHeight = static_cast<float>(height / maxFrame.y);
    margin.x = marginX;
    margin.y = marginY;
    scale = glm::vec2(1.0f, 1.0f);
    angle = 0.0f;
    offset = glm::vec2(0.0f, 0.0f);

    float vertices[] = {
        // positions                        // texture coords
        -frameWidth / 2, -frameHeight / 2,  0.0f, 0.0f,
         frameWidth / 2, -frameHeight / 2,  1.0f, 0.0f,
         frameWidth / 2,  frameHeight / 2,  1.0f, 1.0f,
         frameWidth / 2,  frameHeight / 2,  1.0f, 1.0f,
        -frameWidth / 2,  frameHeight / 2,  0.0f, 1.0f,
        -frameWidth / 2, -frameHeight / 2,  0.0f, 0.0f,
    };

    if (isInit)
    {
        SAFE_DELETE(imageVAO);
    }
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
    shader->setVec2("margin", margin);
    shader->setFloat("alpha", alpha);

    glm::vec3 position = glm::vec3(destX, destY, 0.0f);
    // world transformation
    glm::mat4 planeModel;
    planeModel = glm::translate(planeModel, position);
    planeModel = glm::rotate(planeModel, angle, glm::vec3(0.0f, 0.0f, 1.0f));
    planeModel = glm::scale(planeModel, glm::vec3(scale, 0.0f));
    planeModel = glm::translate(planeModel, glm::vec3(offset, 0.0f));
    shader->setMat4("model", planeModel);

    // bind diffuse map
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sourceTexture->GetID());

    // render the cube
    glBindVertexArray(imageVAO->GetID());
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glBindVertexArray(0);
}

void GLImage::AnimationRender(Shader* /*shader*/, float /*destX*/, float /*destY*/, Animation* /*ani*/)
{
}
