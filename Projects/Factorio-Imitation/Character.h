#pragma once
#include "Entity.h"

class Shader;
class GLImage;
class Inventory;
class Character : public Entity
{
public:
	// 캐릭터의 방향
	enum Direction {
		leftup,	left, leftdown,
		down, rightdown, right,
		rightup, up,
	};

	// 캐릭터의 상태
	enum State {
		IDLE,
		RUNNING,
		MINING,
		END
	};

private:
	std::vector<GLImage*> mainImages;
	std::vector<GLImage*> shadowImages;
	
	float animationSpeed[State::END];
	int animationCurrFrame[State::END];
	glm::vec2 imageAniOffset[State::END];
	glm::vec2 shadowAniOffset[State::END];
	float accumulateTime;

	Direction direction;
	State state;
	float speed;
	Inventory* inventory;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(Shader* lpShader);

	void ChangeState(State state);

	void Idle();
	void Running();
	void Mining();

	glm::vec2* GetLpPosition() { return &position; }
	virtual FRECT GetCollisionFRect() override;
	FRECT GetPickUpFRect();
	Inventory* GetLpInventory();

	Character();
	virtual ~Character() {};
};

