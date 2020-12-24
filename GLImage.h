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
	
	bool isSpecular;
	float alpha;
	float zoom;
	bool isInit;

public:
	//빈 비트맵 이미지를 만든다.
	HRESULT Init(char const* sourceTexture, int width, int height, int maxFrameX = 1, int maxFrameY = 1);
	// 메모리 해제
	void Release();

	void Render(Shader* shader, float destX, float destY, int currFrameX = 0, int currFrameY = 0);
	void AnimationRender(Shader* shader, float destX, float destY, Animation* ani);

	void SetZoom(float zoom)			{ this->zoom = zoom; }
	void SetAlpha(float alpha)			{ this->alpha = alpha; }
	void SetSpecular(bool isSpecular)	{ this->isSpecular = isSpecular; }

	float GetFrameWidth()				{ return this->frameWidth; }
	float GetFrameHeight()				{ return this->frameHeight; }

	GLImage()
		: sourceTexture(nullptr)
		, isSpecular(true)
		, alpha(1.0f)
		, isInit(false)
	{};
};

