#pragma once
#include "GameNode.h"
#include "framework.h"

using namespace glm;

class Shader;
class GLImage;
class EntityManager;
class StructureBuilder : public GameNode
{
	GLImage* itemBuildImage;	// 구조물의 이미지
	bool buildable;				// 구조물을 지을수 있는지?
	float canBuildDistance;		// 구조물의 건설 거리 제한
	ivec2 buildPos;				// 건설할 포지션
	bool active;				// VSB의 활성화 여부
	string entityName;			// 구조물의 이름
	DIRECTION entityDirection;	// 구조물의 방향
	// 참조용
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

