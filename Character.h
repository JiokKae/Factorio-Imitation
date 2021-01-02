#pragma once
#include "Entity.h"

class Shader;
class GLImage;
class Animation;
class Character : public Entity
{
public:
	enum Direction {
		leftup,
		left,
		leftdown,
		down,
		rightdown,
		right,
		rightup,
		up,
	};

	enum State {
		IDLE,
		RUNNING,
		MINING,
		END
	};

private:
	GLImage* image;
	GLImage* shadow;
	Animation* animation;
	
	float animationSpeed[State::END];
	int animationCurrFrame[State::END];
	glm::vec2 imageAniOffset[State::END];
	glm::vec2 shadowAniOffset[State::END];
	float accumulateTime;
	
	glm::vec2 position;
	Direction direction;
	State state;
	float speed;

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
	glm::vec3 GetVec3Direction();
	virtual FRECT GetCollisionFRect() override;

	Character() {};
	virtual ~Character() {};
};

