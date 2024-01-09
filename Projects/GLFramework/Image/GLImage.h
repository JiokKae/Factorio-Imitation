#pragma once
#include <glm/vec2.hpp>
#include <string>

constexpr int USE_BASE_SIZE = -1;

class ShaderProgram;
class Texture;
class VertexArrayObject;
class GLImage final
{
public:
	GLImage(const std::string& textureKey, int maxFrameX = 1, int maxFrameY = 1, float marginX = 0, float marginY = 0, int width = USE_BASE_SIZE, int height = USE_BASE_SIZE);
	~GLImage();

	void Render(ShaderProgram* shader, float destX, float destY, int currFrameX = 0, int currFrameY = 0);

	void SetAngle(float value)			{ angle = value; }
	void SetAlpha(float value)			{ alpha = value; }
	void SetScale(const glm::vec2& value)		{ scale = value; }
	void SetOffset(const glm::vec2& value)		{ offset = value; }

	float GetFrameWidth() const			{ return frameWidth; }
	float GetFrameHeight() const			{ return frameHeight; }
	const glm::vec2& GetMargin() const		{ return margin; }
	const glm::ivec2& GetMaxFrame() const		{ return maxFrame; }
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
