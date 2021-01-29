#pragma once
#include "Structure.h"

class UI;
class Shader;
class GLImage;
class Recipe;
class AssemblingMachine1 : public Structure
{
private:
	GLImage* image;
	GLImage* shadow;
	glm::vec2 imageAniOffset;
	glm::vec2 shadowAniOffset;

	float craftingSpeed;
	glm::ivec2 miningArea;

	Recipe* selectedRecipe;

	float time;

public:
	virtual HRESULT Init(int x, int y, DIRECTION direction, bool temp = false) override;
	virtual void Release();
	virtual void Update();
	virtual void FirstRender(Shader* shader) override;
	virtual void Render(Shader* shader) override;
	virtual void Render(Shader* shader, float posX, float posY) override;

	// event
	virtual void ClickEvent() override;

	virtual string ToString() override;
};

