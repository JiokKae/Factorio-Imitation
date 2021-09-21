#pragma once
#include "../framework.h"

class FreeCamera
{
private:
    // camera Attributes
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    // euler Angles
    float yaw;
    float pitch;
    // camera options
    float movementSpeed;
    float mouseSensitivity;
    float fov;
    float zoomSpeed;
    float lastMouseZDelta;
    bool firstMouse = true;
    std::function<void()> onUpdate;

public:
	void Init(std::function<void()> _onUpdate);
	void Release();
	void Update(float mousezDelta, float dt);

    float GetFov() { return fov; }
    glm::vec3 GerFront() { return front; }
    glm::vec3 GetPosition() { return position; }
    glm::mat4 GetViewMatrix();
};