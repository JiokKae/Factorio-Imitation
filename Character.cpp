#include "Character.h"
#include "GLImage.h"

HRESULT Character::Init()
{
	position = { 0.0f, 0.0f };
	image = new GLImage[State::END];
	image[IDLE].Init("Character/hr-level1_idle", 2024, 928, 22, 8);
	image[RUNNING].Init("Character/hr-level1_running", 1936, 1056, 22, 8);
	//image[IDLE].SetSpecular(false);
	//image[RUNNING].SetSpecular(false);

	shadow = new GLImage[State::END];
	shadow[IDLE].Init("Character/hr-level1_idle_shadow", 3608, 624, 22, 8);
	shadow[RUNNING].Init("Character/hr-level1_running_shadow", 4180, 544, 22, 8);
	shadow[IDLE].SetAlpha(0.6f);
	shadow[RUNNING].SetAlpha(0.6f);

	animationSpeed[IDLE] = 0.01f;
	animationSpeed[RUNNING] = 0.04f;
	
	speed = 250.0f;
	return S_OK;
}

void Character::Release()
{
	SAFE_ARR_DELETE(image);
	SAFE_RELEASE(shadow);
}

void Character::Update()
{
	switch (state)
	{
	case State::IDLE:
		Idle();

		break;
	case State::RUNNING:
		Running();

		break;
	default:
		break;
	}

	
}

void Character::Render(Shader* lpShader)
{
	shadow[state].FrameRender(lpShader, position.x+60, position.y-42, int(timeGetTime() * animationSpeed[state]) % 22, direction);
	image[state].FrameRender(lpShader, position.x, position.y, int(timeGetTime() * animationSpeed[state]) % 22, direction);
}

void Character::Idle()
{
	float scaledspeed = speed * TimerManager::GetSingleton()->GetTimeElapsed();
	glm::vec2 deltaPos;
	if (KeyManager::GetSingleton()->IsStayKeyDown('W'))
		deltaPos.y += scaledspeed;

	if (KeyManager::GetSingleton()->IsStayKeyDown('S'))
		deltaPos.y -= scaledspeed;

	if (KeyManager::GetSingleton()->IsStayKeyDown('A'))
		deltaPos.x -= scaledspeed;

	if (KeyManager::GetSingleton()->IsStayKeyDown('D'))
		deltaPos.x += scaledspeed;

	position += deltaPos;

	if (deltaPos.x != 0 || deltaPos.y != 0)
	{
		Direction newDirection;
		if (deltaPos.x > 0)
		{
			if (deltaPos.y > 0)
				newDirection = Direction::rightup;
			else if (deltaPos.y < 0)
				newDirection = Direction::rightdown;
			else
				newDirection = Direction::right;
		}
		else if (deltaPos.x < 0)
		{
			if (deltaPos.y > 0)
				newDirection = Direction::leftup;
			else if (deltaPos.y < 0)
				newDirection = Direction::leftdown;
			else
				newDirection = Direction::left;
		}
		else
		{
			if (deltaPos.y > 0)
				newDirection = Direction::up;
			else
				newDirection = Direction::down;
		}
		direction = newDirection;
		state = State::RUNNING;
	}

	
}

void Character::Running()
{
	float scaledspeed = speed * TimerManager::GetSingleton()->GetTimeElapsed();
	glm::vec2 deltaPos;
	if (KeyManager::GetSingleton()->IsStayKeyDown('W'))
		deltaPos.y += scaledspeed;

	if (KeyManager::GetSingleton()->IsStayKeyDown('S'))
		deltaPos.y -= scaledspeed;

	if (KeyManager::GetSingleton()->IsStayKeyDown('A'))
		deltaPos.x -= scaledspeed;

	if (KeyManager::GetSingleton()->IsStayKeyDown('D'))
		deltaPos.x += scaledspeed;

	position += deltaPos;

	Direction newDirection = direction;
	if (deltaPos.x > 0)
	{
		if (deltaPos.y > 0)
			newDirection = Direction::rightup;
		else if (deltaPos.y < 0)
			newDirection = Direction::rightdown;
		else
			newDirection = Direction::right;
	}
	else if (deltaPos.x < 0)
	{
		if (deltaPos.y > 0)
			newDirection = Direction::leftup;
		else if (deltaPos.y < 0)
			newDirection = Direction::leftdown;
		else
			newDirection = Direction::left;
	}
	else
	{
		if (deltaPos.y > 0)
			newDirection = Direction::up;
		else if (deltaPos.y < 0)
			newDirection = Direction::down;
		else
			state = State::IDLE;
	}
	direction = newDirection;
}

glm::vec3 Character::GetVec3Direction()
{
	switch (direction)
	{
	case Character::leftup:
		break;
	case Character::left:
		return glm::vec3(-1.0f, 0.0f, 0.0f);
		break;
	case Character::leftdown:
		break;
	case Character::down:
		return glm::vec3(0.0f, -1.0f, 0.0f);
		break;
	case Character::rightdown:
		break;
	case Character::right:
		return glm::vec3(+1.0f, 0.0f, 0.0f);
		break;
	case Character::rightup:
		break;
	case Character::up:
		return glm::vec3(0.0f, +1.0f, 0.0f);
		break;
	default:
		break;
	}
	return glm::vec3(0.0f, +1.0f, 0.0f);
}
