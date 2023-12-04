#pragma once
#include "Entity.h"

class ShaderProgram;
class GLImage;
class Inventory;
class Character : public Entity
{
public:
	enum Direction {
		leftup,	left, leftdown,
		down, rightdown, right,
		rightup, up,
	};

	enum State {
		IDLE,
		RUNNING,
		MINING,
		END
	};

	Character();
	~Character();

	void Update();
	void Render(ShaderProgram* lpShader);

	void ChangeState(State state);

	void Idle();
	void Running();
	void Mining();

	glm::vec2* GetLpPosition() { return &position; }
	FRECT GetCollisionFRect() const override;
	FRECT GetPickUpFRect() const;
	Inventory* GetLpInventory();

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

	Direction GetDirection(float deltaX, float deltaY) const;
	glm::vec2 GetPositionDelta(float distance) const;
};

