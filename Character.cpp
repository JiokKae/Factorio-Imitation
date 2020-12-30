#include "Character.h"
#include "Animation.h"
#include "GLImage.h"

HRESULT Character::Init()
{
	position = { 0.0f, 0.0f };
	direction = Direction::up;
	image = new GLImage[State::END]();
	image[IDLE].Init("Character/hr-level1_idle", 2024, 928, 22, 8);
	imageAniOffset[IDLE] = { 0.0f, 43.0f };
	image[RUNNING].Init("Character/hr-level1_running", 1936, 1056, 22, 8);
	imageAniOffset[RUNNING] = { 0.0f, 43.0f };
	image[MINING].Init("Character/hr-level1_mining_tool", 5096, 1552, 26, 8);
	imageAniOffset[MINING] = { 0.0f, 33.0f };
	//image[IDLE].SetSpecular(false);
	//image[RUNNING].SetSpecular(false);

	shadow = new GLImage[(int)State::END];
	shadow[IDLE].Init("Character/hr-level1_idle_shadow", 3608, 624, 22, 8);
	shadow[IDLE].SetAlpha(0.6f);
	shadowAniOffset[IDLE] = {60.0f, 1.0f};
	shadow[RUNNING].Init("Character/hr-level1_running_shadow", 4180, 544, 22, 8);
	shadow[RUNNING].SetAlpha(0.6f);
	shadowAniOffset[RUNNING] = { 60.0f, 1.0f };
	shadow[MINING].Init("Character/hr-level1_mining_tool_shadow", 7592, 1136, 26, 8);
	shadow[MINING].SetAlpha(0.6f);
	shadowAniOffset[MINING] = { 50.0f, 5.0f };


	animationSpeed[IDLE] = 0.105f;
	animationSpeed[RUNNING] = 0.025f;
	animationSpeed[MINING] = 0.022f;
	
	speed = 566.0f;
	return S_OK;
}

void Character::Release()
{

	SAFE_ARR_DELETE(image);
	SAFE_ARR_DELETE(shadow);
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

	case State::MINING:
		Mining();
		break;

	default:
		break;
	}


	while (accumulateTime > animationSpeed[state])
	{
		animationCurrFrame[state]++;
		accumulateTime-= animationSpeed[state];
	}
	accumulateTime += TimerManager::GetSingleton()->GetTimeElapsed();
}

void Character::Render(Shader* lpShader)
{
	shadow[state].Render(lpShader, position.x + shadowAniOffset[state].x, position.y + shadowAniOffset[state].y, animationCurrFrame[state] % image->GetMaxFrame().x, direction);
	image[state].Render(lpShader, position.x + imageAniOffset[state].x, position.y + imageAniOffset[state].y, animationCurrFrame[state] % image->GetMaxFrame().x, direction);
}

void Character::ChangeState(State state)
{
	this->state = state;
	animationCurrFrame[state] = 0;
	accumulateTime = 0;
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

	if (deltaPos.x != 0 && deltaPos.y != 0)
		position += deltaPos / 1.4142135623f;
	else
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
		ChangeState(State::RUNNING);
	}

	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_RBUTTON))
	{
		ChangeState(State::MINING);
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

	if (deltaPos.x != 0 && deltaPos.y != 0)
		position += deltaPos / 1.4142135623f;
	else
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
			ChangeState(State::IDLE);
	}
	direction = newDirection;

	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_RBUTTON))
	{
		ChangeState(State::MINING);
	}
}

void Character::Mining()
{
	if (KeyManager::GetSingleton()->IsOnceKeyUp(VK_RBUTTON))
	{
		ChangeState(State::IDLE);
	}
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
