#include "GLImage.h"
#include "Texture.h"
#include "Shader.h"

HRESULT GLImage::Init(Texture* sourceTexture, int width, int height, int maxFrameX, int maxFrameY)
{
	this->sourceTexture = sourceTexture;
    
	maxFrame.x = maxFrameX;
	maxFrame.y = maxFrameY;
    frameWidth = (float)width / maxFrame.x/ 100.0f;
    frameHeight = (float)height / maxFrame.y / 100.0f;

    float vertices[] = {
        // positions                        // normals           // texture coords
        -frameWidth / 2, -frameHeight / 2,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         frameWidth / 2, -frameHeight / 2,  0.0f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         frameWidth / 2,  frameHeight / 2,  0.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         frameWidth / 2,  frameHeight / 2,  0.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -frameWidth / 2,  frameHeight / 2,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -frameWidth / 2, -frameHeight / 2,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texcoord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

	return S_OK;
}

void GLImage::Release()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void GLImage::Render(Shader* shader, int destX, int destY, int sizeX, int sizeY)
{
}

void GLImage::FrameRender(Shader* shader, int destX, int destY, int currFrameX, int currFrameY)
{
    shader->use();
    shader->setVec2("currFrame", { currFrameX, currFrameY });
    shader->setVec2("maxFrame", maxFrame);
    glm::vec3 position = glm::vec3(destX / 100.0f, destY / 100.0f, 0.0f);
    // world transformation
    glm::mat4 model = glm::mat4(1.0f);
    shader->setMat4("model", model);

    // bind diffuse map
    shader->setInt("material.diffuse", sourceTexture->GetID());
    glActiveTexture(GL_TEXTURE0 + sourceTexture->GetID());
    glBindTexture(GL_TEXTURE_2D, sourceTexture->GetID());


    // render the cube
    glBindVertexArray(VAO);
    
    glm::mat4 planeModel;
    planeModel = glm::translate(planeModel, position);
    shader->setMat4("model", planeModel);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    glBindVertexArray(0);
}

void GLImage::AnimationRender(Shader* shader, int destX, int destY, Animation* ani)
{
}
