#pragma once
#include "GameNode.h"
#include "framework.h"

using namespace glm;

class Shader;
class Structure;
class EntityManager;
class StructureBuilder : public GameNode
{
	bool buildable;				// �������� ������ �ִ���?
	float canBuildDistance;		// �������� �Ǽ� �Ÿ� ����
	ivec2 buildPos;				// �Ǽ��� ������
	bool active;				// SB�� Ȱ��ȭ ����
	int itemId;
	Structure* tempStructure;	// �ӽ� ������ ��ü

	// ������
	EntityManager* entityManager;
public:
	virtual HRESULT Init(EntityManager*);
	virtual void Release();
	virtual void Update(vec2* playerPos);
	virtual void Render(Shader* shader);

	void Active(int itemId);
	void Deactive();
	bool CheckCanBuild(ivec2 coord, ivec2 coordSize);
	bool IsActive() { return active; }
	StructureBuilder() 
		: tempStructure(nullptr)
	{};
	virtual ~StructureBuilder() {};
};

