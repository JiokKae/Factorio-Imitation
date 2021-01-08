#pragma once
#include "Structure.h"

class UI;
class Shader;
class GLImage;
class BurnerMiningDrill : public Structure
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
	glm::vec2 imageAniOffset[DIRECTION_END];
	glm::vec2 shadowAniOffset[DIRECTION_END];

	STATUS status;
	float miningSpeed;
	glm::ivec2 miningArea;

public:
	virtual HRESULT Init(int x, int y, DIRECTION direction, bool temp = false) override;
	virtual void Release();
	virtual void Update();
	virtual void FirstRender(Shader* lpShader) override;
	virtual void Render(Shader* lpShader) override;

};

