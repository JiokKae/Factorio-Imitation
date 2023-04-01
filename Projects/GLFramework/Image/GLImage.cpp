#include "GLImage.h"
#include "../Texture/Texture.h"
#include "../Texture/TextureManager.h"
#include "../Shader/Shader.h"
#include "../VertexDataObject/VertexArrayObject.h"
#include "../VertexDataObject/VertexBufferObject.h"

GLImage::GLImage(const std::string& textureKey, int maxFrameX, int maxFrameY, float marginX, float marginY, int width, int height)
	: offset(0.0f, 0.0f)
	, scale(1.0f, 1.0f)
	, margin(marginX, marginY)
	, maxFrame(maxFrameX, maxFrameY)
	, sourceTexture{ TextureManager::GetSingleton()->FindTexture(textureKey) }
	, imageVAO{ nullptr }
	, frameWidth{ 0.0f }
	, frameHeight{ 0.0f }
	, alpha{ 1.0f }
	, angle{ 0.0f }
{
	if (sourceTexture == nullptr)
		return;
	// Load base width, height;
	width = (width == -1) ? sourceTexture->GetWidth() : width;
	height = (height == -1) ? sourceTexture->GetHeight() : height;

	frameWidth = static_cast<float>(width / maxFrame.x);
	frameHeight = static_cast<float>(height / maxFrame.y);

	float vertices[] = {
		// positions                        // texture coords
		-frameWidth / 2, -frameHeight / 2,  0.0f, 0.0f,
		 frameWidth / 2, -frameHeight / 2,  1.0f, 0.0f,
		 frameWidth / 2,  frameHeight / 2,  1.0f, 1.0f,
		 frameWidth / 2,  frameHeight / 2,  1.0f, 1.0f,
		-frameWidth / 2,  frameHeight / 2,  0.0f, 1.0f,
		-frameWidth / 2, -frameHeight / 2,  0.0f, 0.0f,
	};

	// TODO: VBO할당 해제 잘 하고 있는지 확인
	imageVAO = new VAO();
	VBO* posTexcoordVBO = new VBO();
	posTexcoordVBO->SetData(sizeof(vertices), vertices, GL_STATIC_DRAW);
	imageVAO->AddVBO(0, posTexcoordVBO, 4);
}

GLImage::~GLImage()
{
	SAFE_DELETE(imageVAO);
}

void GLImage::Render(ShaderProgram* shader, float destX, float destY, int currFrameX, int currFrameY)
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
