#pragma once
#include "GameNode.h"
#include "framework.h"

using namespace glm;

class Shader;
class GLImage;
class EntityManager;
class StructureBuilder : public GameNode
{
	GLImage* itemBuildImage;	// �������� �̹���
	bool buildable;				// �������� ������ �ִ���?
	float canBuildDistance;		// �������� �Ǽ� �Ÿ� ����
	ivec2 buildPos;				// �Ǽ��� ������
	bool active;				// VSB�� Ȱ��ȭ ����
	string entityName;			// �������� �̸�
	DIRECTION entityDirection;	// �������� ����
	// ������
	EntityManager* entityManager;
public:
	virtual HRESULT Init(EntityManager*);
	virtual void Release();
	virtual void Update(vec2* playerPos);
	virtual void Render(Shader* shader);

	void Active();
	void Deactive();
	bool CheckCanBuild(ivec2 coord, ivec2 coordSize);

	StructureBuilder() {};
	virtual ~StructureBuilder() {};
};

