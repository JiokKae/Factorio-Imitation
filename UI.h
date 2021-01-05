#pragma once
#include "GameNode.h"
#include "framework.h"

class Shader;
class GLImage;
class UI : public GameNode
{
protected:
	UI* parent;					// �θ�
    GLImage* image;				// ���� �̹���
	glm::vec2 localPosition;	// UI�� ���� ��ġ
	bool active;				// Ȱ��ȭ ����

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(Shader* lpShader);

	void SetParent(UI* parent)					{ this->parent = parent; }
	void SetActive(bool active)					{ this->active = active; }
	void TurnActive()							{ this->active = !active; }
	void SetLocalPosition(glm::vec2 position)	{ this->localPosition = position; }

	bool IsActive()					{ return this->active; }
	float GetWidth();
	float GetHeight();
	glm::vec2 GetPosition()			{ if (parent) return parent->GetPosition() + localPosition; else return localPosition; }
	glm::vec2 GetLocalPosition()	{ return localPosition; }
	FRECT GetFrect();

	virtual void OnMouse() {};

	// Ŭ�� �̺�Ʈ
	virtual void OnClick() {};

	UI()
		: image(nullptr)
		, parent(nullptr)
		, active(true)
	{};
};

