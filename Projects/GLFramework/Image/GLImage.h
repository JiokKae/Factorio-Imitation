#pragma once
#include "../framework.h"

class Shader;
class Texture;
class VertexArrayObject;
class GLImage final
{
public:
	GLImage(const std::string& textureKey, int maxFrameX = 1, int maxFrameY = 1, float marginX = 0, float marginY = 0, int width = -1, int height = -1);
	~GLImage();

	void Render(Shader* shader, float destX, float destY, int currFrameX = 0, int currFrameY = 0);

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

private:
	glm::vec2 offset;
	glm::vec2 scale;
	glm::vec2 margin;
	glm::ivec2 maxFrame;
	Texture* sourceTexture;
	VertexArrayObject* imageVAO;
	float frameWidth;
	float frameHeight;
	float alpha;	// 이미지의 투명도
	float angle;	// 이미지의 회전 각도
};
