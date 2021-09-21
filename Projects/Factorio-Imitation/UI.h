#pragma once
#include "GameNode.h"
#include "framework.h"

class Shader;
class GLImage;
class UI : public GameNode
{
protected:
	UI* parent;			// 부모
	GLImage* image;			// 메인 이미지
	glm::vec2 localPosition;	// UI의 지역 위치
	bool active;			// 활성화 여부

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(Shader* lpShader);

	void SetParent(UI* parent)			{ this->parent = parent; }
	void SetActive(bool active)			{ this->active = active; }
	void TurnActive()				{ this->active = !active; }
	void SetLocalPosition(glm::vec2 position)	{ this->localPosition = position; }

	bool IsActive() { return this->active; }
	float GetWidth();
	float GetHeight();
	glm::vec2 GetPosition()		{ if (parent) return parent->GetPosition() + localPosition; else return localPosition; }
	glm::vec2 GetLocalPosition()	{ return localPosition; }
	FRECT GetFrect();

	virtual void OnMouse() {};

	// 클릭 이벤트
	virtual void OnClick(int key) {};

	UI()
		: image(nullptr)
		, parent(nullptr)
		, active(true)
	{};
};

