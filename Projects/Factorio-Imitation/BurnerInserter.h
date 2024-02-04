#pragma once
#include "Structure.h"

#include <memory>

namespace Degree 
{
	inline float Sin(float degree);
	inline float Cos(float degree);
	inline float Rad(float degree);
}

class Tile;
class BurnerInserter : public Structure
{
public:
	class HandPart
	{
	public:
		HandPart(const std::string& textureKey, const glm::vec2& scale, const glm::vec2& offset, float degree, float innerLength);

		void Update(float handDegree);
		void Render(ShaderProgram* shader, float posX, float posY);

		float EndpointPosX() const;
		float EndpointPosY() const;
	private:
		const std::unique_ptr<GLImage> image;
		const float degree;
		const float innerLength;
		const float baseScaleY;
		float renderDegree;
	};

public:
	BurnerInserter();

	virtual HRESULT Init(int x, int y, DIRECTION direction, bool temp = false) override;
	virtual void Update() override;

	virtual void Render(ShaderProgram* lpShader) override;
	virtual void LateRender(ShaderProgram* lpShader) override;

	virtual void RenderInScreen(ShaderProgram* shader, float posX, float posY) override;

	virtual std::string ToString() override;

private:
	const std::unique_ptr<GLImage> platformImage;
	const std::unique_ptr<GLImage> allItemsImage;

	HandPart handBase;
	HandPart handOpen;

	float handDegree;
	const float handDegreeSpeed;

	Tile* lpDirectionTile = nullptr;
	
	ItemInfo handItem;

	static constexpr float DESTINATION_ANGLES[4] = { 0.0f, -90.0f, -180.0f, -270.0f };
};
