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

public:
	//빈 비트맵 이미지를 만든다.
	HRESULT Init(Texture* sourceTexture, int width, int height, int maxFrameX = 1, int maxFrameY = 1);
	// 메모리 해제
	void Release();

	void Render(Shader* shader, int destX, int destY, int sizeX, int sizeY);
	void FrameRender(Shader* shader, int destX, int destY, int currFrameX, int currFrameY);
	void AnimationRender(Shader* shader, int destX, int destY, Animation* ani);

	GLImage()
		: sourceTexture(nullptr)
	{};
};

