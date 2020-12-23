#pragma once
#include "GameNode.h"
class Shader;
class GLImage;
class Character : public GameNode
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
		END
	};

private:
	GLImage* image;
	float animationSpeed[State::END];
	GLImage* shadow;
	glm::vec2 position;
	Direction direction;
	State state;
	float speed;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(Shader* lpShader);

	void Idle();
	void Running();

	glm::vec2* GetLpPosition() { return &position; }
	glm::vec3 GetVec3Direction();
	

	Character() {};
	virtual ~Character() {};
};

