#include "Entity.h"

Entity::Entity()
	: position{}
	, passable{ false }
{
}

Entity::~Entity()
{
}

HRESULT Entity::Init()
{
	HRESULT result = GameNode::Init();
	return result;
}

void Entity::Release()
{
	GameNode::Release();

	position = Vec2();
	passable = false;
}

void Entity::Update()
{
	GameNode::Update();
}

void Entity::SetPosition(const Vec2& newPosition)
{
	this->position = newPosition;
}

FRECT Entity::GetCollisionFRect() const
{
	return FRECT{};
}

FRECT Entity::GetFRect() const
{
	return FRECT{};
}

const Vec2& Entity::GetPosition() const
{
	return this->position;
}

bool Entity::IsPassable() const
{
	return this->passable;
}
