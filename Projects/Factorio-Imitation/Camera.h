#pragma once
#include "framework.h"

class Camera
{
private:
	// camera Attributes
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	// camera options
	float movementSpeed;
	float zoom;
	float zoomSpeed;
	float lastMouseZDelta;
	bool firstMouse = true;

	// target
	glm::vec2* target;

public:
	HRESULT Init();
	void Release();
	void Update();

	float GetZoom()		{ return powf(2, zoom); }
	glm::vec3 GerFront()	{ return front; }
	glm::vec3 GetPosition() { return position; }
	glm::mat4 GetViewMatrix();
	RECT GetRect(int winWidth, int winHeight);
	FRECT GetFRect(int winWidth, int winHeight);

	void SetTarget(glm::vec2* targetPos) { this->target = targetPos; }
	void SetPosition(glm::vec3 position) { this->position = position; }

	Camera()
		: target()
		, lastMouseZDelta()
		, movementSpeed()
		, zoom()
		, zoomSpeed()
	{};
};