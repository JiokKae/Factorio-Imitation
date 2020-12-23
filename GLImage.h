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
	//�� ��Ʈ�� �̹����� �����.
	HRESULT Init(char const* sourceTexture, int width, int height, int maxFrameX = 1, int maxFrameY = 1);
	// �޸� ����
	void Release();

	void Render(Shader* shader, float destX, float destY);
	void FrameRender(Shader* shader, float destX, float destY, int currFrameX, int currFrameY);
	void AnimationRender(Shader* shader, float destX, float destY, Animation* ani);

	void SetZoom(float zoom)			{ this->zoom = zoom; }
	void SetAlpha(float alpha)			{ this->alpha = alpha; }
	void SetSpecular(bool isSpecular)	{ this->isSpecular = isSpecular; }

	GLImage()
		: sourceTexture(nullptr)
		, isSpecular(true)
		, alpha(1.0f)
		, isInit(false)
	{};
};

