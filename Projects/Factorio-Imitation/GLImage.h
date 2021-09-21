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
	glm::vec2 offset;
	float alpha;			// 이미지의 투명도
	glm::vec2 scale;
	float angle;			// 이미지의 회전 각도
	glm::vec2 margin;
	bool isInit;

public:
	// 이미지 초기화
	HRESULT Init(char const* sourceTexture, int maxFrameX = 1, int maxFrameY = 1, float marginX = 0, float marginY = 0, int width = -1, int height = -1);

	// 메모리 해제
	void Release();

	void Render(Shader* shader, float destX, float destY, int currFrameX = 0, int currFrameY = 0);
	void AnimationRender(Shader* shader, float destX, float destY, Animation* ani);

	void SetAngle(float value)			{ angle = value; }
	void SetAlpha(float value)			{ alpha = value; }
	void SetScale(glm::vec2 value)			{ scale = value; }
	void SetOffset(glm::vec2 value)			{ offset = value; }
	void SetSourceTexture(Texture* texture)		{ sourceTexture = texture; }

	glm::vec2 GetScale()				{ return scale; }
	float GetFrameWidth()				{ return frameWidth; }
	float GetFrameHeight()				{ return frameHeight; }
	glm::vec2 GetMargin()				{ return margin; }
	glm::ivec2 GetMaxFrame()			{ return maxFrame; }
	float GetZoomedFrameWidth()			{ return frameWidth * scale.x; }
	float GetZoomedFrameHeight()			{ return frameHeight * scale.y; }
	Texture* GetLpSourceTexture()			{ return sourceTexture; }

	GLImage()
		: sourceTexture(nullptr)
		, imageVAO(nullptr)
		, angle(0.0f)
		, alpha(1.0f)
		, frameWidth(0.0f)
		, frameHeight(0.0f)
		, isInit(false)
	{};
};

