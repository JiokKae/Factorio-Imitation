#include "FreeCamera.h"

void FreeCamera::Init(std::function<void()> _onUpdate)
{
	position = glm::vec3(0.0f, 0.0f, 3.0f);
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);

	yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
	pitch = 0.0f;
	fov = 45.0f;

	movementSpeed = 1.5f;
	this->mouseSensitivity = 0.03f;
	zoomSpeed = 2.0f;

	onUpdate = _onUpdate;
}

void FreeCamera::Release()
{
}

/*onUpdate
*		float cameraSpeed = movementSpeed * dt; // adjust accordingly
* 
	if (KeyManager::GetSingleton()->IsStayKeyDown('W'))
		position += front * cameraSpeed;

	if (KeyManager::GetSingleton()->IsStayKeyDown('S'))
		position -= front * cameraSpeed;

	if (KeyManager::GetSingleton()->IsStayKeyDown('A'))
		position -= glm::normalize(glm::cross(front, up)) * cameraSpeed;

	if (KeyManager::GetSingleton()->IsStayKeyDown('D'))
		position += glm::normalize(glm::cross(front, up)) * cameraSpeed;

	RECT winRect = GetWindowRect();

	float xoffset = g_ptMouse.x - WINSIZE_X / 2;
	float yoffset = g_ptMouse.y - WINSIZE_Y / 2; // y 좌표의 범위는 밑에서부터 위로가기 때문에 반대로 바꿉니다.
	SetCursorPos(winRect.left+ WINSIZE_X / 2, winRect.top + WINSIZE_Y / 2);

		xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

		yaw += xoffset;
	pitch = glm::clamp(pitch + yoffset, -89.0f, 89.0f);

*/
void FreeCamera::Update(float mousezDelta, float /*dt*/)	// g_mousezDelta, TimerManager::GetSingleton()->GetTimeElapsed()
{
	// fov
	float offset = lastMouseZDelta - mousezDelta;
	lastMouseZDelta = mousezDelta;
	fov += offset / 120.0f * zoomSpeed;
	fov = glm::clamp(fov, 1.0f, 45.0f);

	onUpdate();

	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	front = glm::normalize(front);
	glm::lookAt(position, position + front, up);
}

glm::mat4 FreeCamera::GetViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}
