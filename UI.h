#pragma once
#include "GameNode.h"
#include "framework.h"

class Shader;
class GLImage;
class UI : public GameNode
{
protected:
    GLImage* image;
    glm::vec2 position;
	bool active;
public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(Shader* lpShader);

	void SetActive(bool active) { this->active = active; }

	bool IsActive() { return this->active; }
	UI() 
		: image(nullptr)
		, active(true)
	{};
};

