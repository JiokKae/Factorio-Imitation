#pragma once
#include "framework.h"

class Shader;
class Texture;
class Animation;
class VertexArrayObject;
class GLImage
{
	Texture* sourceTexture;

	glm::ivec2 maxFrame;
	float frameWidth;
	float frameHeight;
	VertexArrayObject* imageVAO;
	glm::vec2 offset;				// �̹����� ������
	float alpha;					// �̹����� ����
	glm::vec2 scale;				// �̹����� ������
	float angle;					// �̹����� ȸ�� ����
	glm::vec2 margin;				// �̹����� ����
	bool isInit;

public:
	// �̹��� �ʱ�ȭ
	HRESULT Init(char const* sourceTexture, int maxFrameX = 1, int maxFrameY = 1, float marginX = 0, float marginY = 0, int width = -1, int height = -1);

	// �޸� ����
	void Release();

	void Render(Shader* shader, float destX, float destY, int currFrameX = 0, int currFrameY = 0);
	void AnimationRender(Shader* shader, float destX, float destY, Animation* ani);

	void SetAngle(float angle)				{ this->angle = angle; }
	void SetAlpha(float alpha)				{ this->alpha = alpha; }
	void SetScale(glm::vec2 scale)			{ this->scale = scale; }
	void SetOffset(glm::vec2 offset)		{ this->offset = offset; }
	void SetSourceTexture(Texture* texture) { this->sourceTexture = texture; }

	glm::vec2 GetScale()				{ return scale; }
	float GetFrameWidth()				{ return frameWidth; }
	float GetFrameHeight()				{ return frameHeight; }
	glm::vec2 GetMargin()				{ return margin; }
	glm::ivec2 GetMaxFrame()			{ return maxFrame; }
	float GetZoomedFrameWidth()			{ return frameWidth * scale.x; }
	float GetZoomedFrameHeight()		{ return frameHeight * scale.y; }
	Texture* GetLpSourceTexture()		{ return sourceTexture; }

	GLImage()
		: sourceTexture(nullptr)
		, alpha(1.0f)
		, isInit(false)
	{};
};

