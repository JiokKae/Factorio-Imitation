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

	float alpha;
	float zoom;
	bool isInit;

public:
	//빈 비트맵 이미지를 만든다.
	HRESULT Init(char const* sourceTexture, int maxFrameX = 1, int maxFrameY = 1, int width = -1, int height = -1);

	// 메모리 해제
	void Release();

	void Render(Shader* shader, float destX, float destY, int currFrameX = 0, int currFrameY = 0);
	void AnimationRender(Shader* shader, float destX, float destY, Animation* ani);

	void SetZoom(float zoom)			{ this->zoom = zoom; }
	void SetAlpha(float alpha)			{ this->alpha = alpha; }

	float GetZoom()						{ return zoom; }
	float GetFrameWidth()				{ return frameWidth; }
	float GetFrameHeight()				{ return frameHeight; }
	glm::ivec2 GetMaxFrame()			{ return maxFrame; }
	float GetZoomedFrameWidth()			{ return frameWidth * zoom; }
	float GetZoomedFrameHeight()		{ return frameHeight * zoom; }
	Texture* GetLpSourceTexture()		{ return sourceTexture; }

	GLImage()
		: sourceTexture(nullptr)
		, alpha(1.0f)
		, isInit(false)
	{};
};

