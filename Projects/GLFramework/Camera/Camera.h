#pragma once

#include "../framework.h"

class Camera
{
public:
	Camera();

	void Update(float mousezDelta);

	void SetFollowTarget(glm::vec2* targetPos);
	void SetPosition(const glm::vec3& newPosition);

	float GetZoom() const;
	glm::mat4 GetViewMatrix() const;
	const glm::vec3& GetPosition() const;
	RECT GetRect(int winWidth, int winHeight) const;
	FRECT GetFRect(int winWidth, int winHeight) const;

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;

	float zoom;
	float zoomSpeed;
	float zoomMin;
	float zoomMax;
	float lastMouseZDelta;

	glm::vec2* followTarget;
};
