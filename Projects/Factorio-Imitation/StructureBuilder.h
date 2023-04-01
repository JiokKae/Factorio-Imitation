#pragma once
#include "../2DFramework/GameNode.h"
#include "framework.h"

using namespace glm;

class Shader;
class Structure;
class EntityManager;
class StructureBuilder : public GameNode
{
	bool buildable;				// 구조물을 지을수 있는지?
	float canBuildDistance;		// 구조물의 건설 거리 제한
	ivec2 buildPos;				// 건설할 포지션
	bool active;				// SB의 활성화 여부
	int itemId;
	Structure* tempStructure;	// 임시 구조물 객체

	// 참조용
	EntityManager* entityManager;
public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update(vec2* playerPos);
	virtual void Render(ShaderProgram* shader);

	void Active(int itemId);
	void Deactive();
	bool CheckCanBuild(ivec2 coord, ivec2 coordSize);
	bool IsActive() { return active; }
	StructureBuilder() 
		: tempStructure(nullptr)
	{};
	virtual ~StructureBuilder() {};
};

