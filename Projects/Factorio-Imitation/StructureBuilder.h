#pragma once
#include "../2DFramework/GameNode.h"
#include "framework.h"

using namespace glm;

class Shader;
class Structure;
class StructureBuilder : public GameNode
{
public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update(vec2* playerPos);
	virtual void Render(ShaderProgram* shader);

	void Activate(int itemId);
	void Deactivate();
	bool IsActive() { return active; }

private:
	bool CheckCanBuild(ivec2 coord, ivec2 coordSize);
	bool IsOddNumber(int num) const;
	std::string GetBuildSoundName(const glm::ivec2& coordSize) const;

	float canBuildDistance;			// 구조물의 건설 거리 제한
	Structure* tempStructure = nullptr;
	int itemId;
	bool buildable;				// 구조물을 지을수 있는지?
	bool active;				// StructureBuilder의 활성화 여부
};
