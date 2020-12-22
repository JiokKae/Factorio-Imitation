#pragma once
#include "framework.h"

class Shader;
class Texture;
class Animation;
class GLImage
{
	Texture* sourceTexture;

	glm::vec2 maxFrame;
	float frameWidth;
	float frameHeight;
	unsigned int VAO;
	unsigned int VBO;
	
	bool isSpecular = true;
	bool isInit;

public:
	//빈 비트맵 이미지를 만든다.
	HRESULT Init(Texture* sourceTexture, int width, int height, int maxFrameX = 1, int maxFrameY = 1);
	// 메모리 해제
	void Release();

	void Render(Shader* shader, float destX, float destY, int sizeX, int sizeY);
	void FrameRender(Shader* shader, float destX, float destY, int currFrameX, int currFrameY);
	void AnimationRender(Shader* shader, float destX, float destY, Animation* ani);

	void SetSpecular(bool isSpecular) { this->isSpecular = isSpecular; }

	GLImage()
		: sourceTexture(nullptr)
	{};
};

