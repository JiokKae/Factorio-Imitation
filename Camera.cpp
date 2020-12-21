#include "Camera.h"

HRESULT Camera::Init()
{
	position = glm::vec3(0.0f, 0.0f, 3.0f);
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);

	fov = 45.0f;

	movementSpeed = 15.0f;
	zoomSpeed = 2.0f;

	return S_OK;
}

void Camera::Release()
{
}

void Camera::Update()
{
	// fov
	float offset = lastMouseZDelta - g_mousezDelta;
	lastMouseZDelta = g_mousezDelta;
	fov += offset / 120.0f * zoomSpeed;
	fov = Clamp(fov, 5.0f, 89.0f);

	float cameraSpeed = movementSpeed * TimerManager::GetSingleton()->GetTimeElapsed(); // adjust accordingly

	if (KeyManager::GetSingleton()->IsStayKeyDown('W'))
		position += up * cameraSpeed;

	if (KeyManager::GetSingleton()->IsStayKeyDown('S'))
		position -= up * cameraSpeed;

	if (KeyManager::GetSingleton()->IsStayKeyDown('A'))
		position -= glm::normalize(glm::cross(front, up)) * cameraSpeed;

	if (KeyManager::GetSingleton()->IsStayKeyDown('D'))
		position += glm::normalize(glm::cross(front, up)) * cameraSpeed;

	glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}
