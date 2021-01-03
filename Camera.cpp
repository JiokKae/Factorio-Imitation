#include "Camera.h"

HRESULT Camera::Init()
{
	position = glm::vec3(0.0f, 0.0f, 3.0f);
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);

	movementSpeed = 15.0f;
	zoom = 0.0f;
	zoomSpeed = 0.1f;

	return S_OK;
}

void Camera::Release()
{
	ReleaseSingleton();
}

void Camera::Update()
{
	// zoom
	float offset = lastMouseZDelta - g_mousezDelta;
	lastMouseZDelta = g_mousezDelta;
	zoom -= offset / 120.0f * zoomSpeed;
	zoom = Clamp(zoom, -2.3f, 0.7f);

	if (target)
	{
		position.x = (*target).x;
		position.y = (*target).y;
	}
	
	float cameraSpeed = movementSpeed * TimerManager::GetSingleton()->GetTimeElapsed(); // adjust accordingly
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

RECT Camera::GetRect(int winWidth, int winHeight)
{
	RECT rect;

	rect.left = position.x - (winWidth / 2) / GetZoom();
	rect.right = position.x + (winWidth / 2) / GetZoom();
	rect.top = position.y + (winHeight / 2) / GetZoom();
	rect.bottom = position.y - (winHeight / 2) / GetZoom();

	return rect;
}
