﻿#include "Character.h"
#include "../GLFramework/Image/GLImage.h"
#include "Inventory.h"
#include "Structure.h"
#include "Tile.h"
#include "TransportBelt.h"

Character::Character()
	: mainImages(State::END)
	, shadowImages(State::END)
	, accumulateTime{ 0.0f }
	, direction{ Direction::up }
	, speed{ 566.0f }
{
	position = { 0.0f, 0.0f };

	mainImages[IDLE] = new GLImage("Entity/Character-level1_idle", 22, 8);
	imageAniOffset[IDLE] = { 0.0f, 43.0f };
	mainImages[RUNNING] = new GLImage("Entity/Character-level1_running", 22, 8);
	imageAniOffset[RUNNING] = { 0.0f, 43.0f };
	mainImages[MINING] = new GLImage("Entity/Character-level1_mining_tool", 26, 8);
	imageAniOffset[MINING] = { 0.0f, 33.0f };

	shadowImages[IDLE] = new GLImage("Entity/Character-level1_idle_shadow", 22, 8);
	shadowImages[IDLE]->SetAlpha(0.6f);
	shadowAniOffset[IDLE] = { 60.0f, 1.0f };
	shadowImages[RUNNING] = new GLImage("Entity/Character-level1_running_shadow", 22, 8);
	shadowImages[RUNNING]->SetAlpha(0.6f);
	shadowAniOffset[RUNNING] = { 60.0f, 1.0f };
	shadowImages[MINING] = new GLImage("Entity/Character-level1_mining_tool_shadow", 26, 8);
	shadowImages[MINING]->SetAlpha(0.6f);
	shadowAniOffset[MINING] = { 50.0f, 5.0f };

	animationSpeed[IDLE] = 0.105f;
	animationSpeed[RUNNING] = 0.025f;
	animationSpeed[MINING] = 0.022f;

	inventory = new Inventory();

	inventory->AddItem(ItemInfo(COAL, 500));
	inventory->AddItem(ItemInfo(BURNER_MINING_DRILL, 40));
	inventory->AddItem(ItemInfo(IRON_PLATE, 30));
	inventory->AddItem(ItemInfo(IRON_PLATE, 30));
	inventory->AddItem(ItemInfo(ASSEMBLING_MACHINE_1, 30));
	inventory->AddItem(ItemInfo(TRANSPORT_BELT, 200));
	inventory->AddItem(ItemInfo(BURNER_INSERTER, 123));
	inventory->AddItem(ItemInfo(WOOD, 300));
	inventory->AddItem(ItemInfo(STONE_FURNACE, 200));
	inventory->AddItem(ItemInfo(SOLID_FUEL, 30));
	inventory->AddItem(ItemInfo(ROCKER_FUEL, 20));
	inventory->AddItem(ItemInfo(NUCLEAR_FUEL, 15));
	for (int i = 0; i < ItemId::END; i++)
	{
		inventory->AddItem(ItemInfo(i, 200));
	}
}

Character::~Character()
{
	SAFE_DELETE(inventory);

	for (auto& image : mainImages)
	{
		SAFE_DELETE(image);
	}

	for (auto& image : shadowImages)
	{
		SAFE_DELETE(image);
	}
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

	glm::ivec2 coord = POS_TO_COORD(position);
	Structure* structure = TileManager::GetSingleton()->GetLpTile(coord.x, coord.y)->GetLpSturcture();
	if (structure && TransportBelt::IsTransportBelt(structure->GetItemId()))
	{
		static_cast<TransportBelt*>(structure)->FlowItem(this, false);
	}

	while (accumulateTime > animationSpeed[state])
	{
		animationCurrFrame[state]++;
		accumulateTime-= animationSpeed[state];
	}
	accumulateTime += TimerManager::GetSingleton()->GetTimeElapsed();
}

void Character::Render(ShaderProgram* lpShader)
{
	shadowImages[state]->Render(lpShader, position.x + shadowAniOffset[state].x, position.y + shadowAniOffset[state].y, 
		animationCurrFrame[state] % shadowImages[state]->GetMaxFrame().x, direction);
	mainImages[state]->Render(lpShader, position.x + imageAniOffset[state].x, position.y + imageAniOffset[state].y, 
		animationCurrFrame[state] % mainImages[state]->GetMaxFrame().x, direction);
}

void Character::ChangeState(State _state)
{
	state = _state;
	animationCurrFrame[state] = 0;
	accumulateTime = 0;
}

void Character::Idle()
{
	glm::vec2 delta{ GetPositionDelta(speed * TimerManager::GetSingleton()->GetTimeElapsed()) };
	position += delta;

	if (delta != glm::vec2{})
	{	
		direction = GetDirection(delta.x, delta.y);
		ChangeState(State::RUNNING);
	}

	/*
	* // TODO: 광석 채굴 구현
	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_RBUTTON))
	{
		ChangeState(State::MINING);
	}
	*/
}

void Character::Running()
{
	glm::vec2 delta{ GetPositionDelta(speed * TimerManager::GetSingleton()->GetTimeElapsed()) };
	position += delta;

	if (delta != glm::vec2{})
	{
		direction = GetDirection(delta.x, delta.y);
	}
	else
	{
		ChangeState(State::IDLE);
	}
}

void Character::Mining()
{
	if (KeyManager::GetSingleton()->IsOnceKeyUp(VK_RBUTTON))
	{
		ChangeState(State::IDLE);
	}
}

FRECT Character::GetCollisionFRect() const
{
	return FRECT{
		.left = position.x - (TILE_SIZE / 4 * (4.0f / 5.0f)),
		.top = position.y + (TILE_SIZE / 4 * (4.0f / 5.0f)),
		.right = position.x + (TILE_SIZE / 4 * (4.0f / 5.0f)),
		.bottom = position.y - (TILE_SIZE / 4 * (4.0f / 5.0f)),
	};
}

FRECT Character::GetPickUpFRect() const
{
	return FRECT{
		.left = position.x - (TILE_SIZE / 2),
		.top = position.y + (TILE_SIZE / 2),
		.right = position.x + (TILE_SIZE / 2),
		.bottom = position.y - (TILE_SIZE / 2),
	};
}

Inventory* Character::GetLpInventory()
{
	return inventory;
}

Character::Direction Character::GetDirection(float deltaX, float deltaY) const
{
	if (deltaX > 0)
	{
		if (deltaY > 0)
			return Direction::rightup;
		if (deltaY < 0)
			return Direction::rightdown;
		return Direction::right;
	}
	if (deltaX < 0)
	{
		if (deltaY > 0)
			return Direction::leftup;
		if (deltaY < 0)
			return Direction::leftdown;
		return Direction::left;
	}
	if (deltaY > 0)
		return Direction::up;

	return Direction::down;
}

glm::vec2 Character::GetPositionDelta(float distance) const
{
	glm::vec2 positionDelta{ 0.0f, 0.0f };

	if (KeyManager::GetSingleton()->IsStayKeyDown('W'))
		positionDelta.y += distance;

	if (KeyManager::GetSingleton()->IsStayKeyDown('S'))
		positionDelta.y -= distance;

	if (KeyManager::GetSingleton()->IsStayKeyDown('A'))
		positionDelta.x -= distance;

	if (KeyManager::GetSingleton()->IsStayKeyDown('D'))
		positionDelta.x += distance;

	if (positionDelta.x != 0 && positionDelta.y != 0)
		positionDelta /= 1.4142135623f;

	return positionDelta;
}
