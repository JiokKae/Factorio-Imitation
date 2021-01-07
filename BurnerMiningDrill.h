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
	virtual HRESULT Init(int x, int y);
	virtual void Release();
	virtual void Update();
	virtual void Render(Shader* lpShader);

	FRECT GetFRect();
	virtual FRECT GetCollisionFRect() override;
};

