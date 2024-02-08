#include "Camera.h"

Camera::Camera()
	: position{ 0.0f, 0.0f, 3.0f }
	, front{ 0.0f, 0.0f, -1.0f }
	, up{ 0.0f, 1.0f, 0.0f }
	, followTarget{ nullptr }
	, lastMouseZDelta{ 0.0f }
	, zoom{ 0.0f }
	, zoomSpeed{ 0.1f }
	, zoomMin{ -2.3f }
	, zoomMax{ 0.7f }
{
}

void Camera::Update(float mouseZDelta)
{
	const float offset = lastMouseZDelta - mouseZDelta;
	zoom -= offset / 120.0f * zoomSpeed;
	zoom = glm::clamp(zoom, zoomMin, zoomMax);

	lastMouseZDelta = mouseZDelta;

	if (followTarget)
	{
		position.x = (*followTarget).x;
		position.y = (*followTarget).y;
	}
}

float Camera::GetZoom() const
{
	return ::powf(2.0f, zoom);
}

const glm::vec3& Camera::GetPosition() const
{
	return position;
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::GetProjectionMatrix(float width, float height) const
{
	const float halfWidth = (width * 0.5f) / GetZoom();
	const float halfHeight = (height * 0.5f) / GetZoom();
	return glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight);
}

RECT Camera::GetRect(int winWidth, int winHeight) const
{
	return RECT {
		.left = static_cast<LONG>(position.x - (winWidth / 2) / GetZoom()),
		.top = static_cast<LONG>(position.y + (winHeight / 2) / GetZoom()),
		.right = static_cast<LONG>(position.x + (winWidth / 2) / GetZoom()),
		.bottom = static_cast<LONG>(position.y - (winHeight / 2) / GetZoom()),
	};
}

FRECT Camera::GetFRect(int winWidth, int winHeight) const
{
	return FRECT {
		.left = position.x - (winWidth / 2) / GetZoom(),
		.top = position.y + (winHeight / 2) / GetZoom(),
		.right = position.x + (winWidth / 2) / GetZoom(),
		.bottom = position.y - (winHeight / 2) / GetZoom(),
	};
}

void Camera::SetFollowTarget(glm::vec2* targetPos)
{
	followTarget = targetPos;
}

void Camera::SetPosition(const glm::vec3& newPosition)
{
	position = newPosition;
}
