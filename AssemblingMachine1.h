#pragma once
#include "Structure.h"

class UI;
class Shader;
class GLImage;
class AssemblingMachine1 : public Structure
{
public:
	enum STATUS {
		NOPOWER,
		WAITING_SPACE,
		DESTORY,
		END
	};

private:
	GLImage* image;
	GLImage* shadow;
	glm::vec2 imageAniOffset;
	glm::vec2 shadowAniOffset;

	STATUS status;
	float miningSpeed;
	glm::ivec2 miningArea;

public:
	virtual HRESULT Init(int x, int y, DIRECTION direction, bool temp = false) override;
	virtual void Release();
	virtual void Update();
	virtual void FirstRender(Shader* shader) override;
	virtual void Render(Shader* shader) override;
	virtual void Render(Shader* shader, float posX, float posY) override;
};

