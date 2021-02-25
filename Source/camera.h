#pragma once
#include <glm/glm.hpp>

const float CAM_SENSITIVITY = 0.003f;
const float CAM_SPEED = 4.f;

struct Camera
{
	float yaw;
	float pitch;

	glm::vec3 position = glm::vec3(0.f, 0.f, 5.f);

	bool perspective = true;
	float fov = 60.f;
	float aspect = 1.f;

	float near = 0.1f;
	float far = 100.f;

	float orthoHeight = 5.f;
};

void CameraUpdate();
glm::mat4 CameraMatrix();

glm::quat CameraQuaternion();
glm::vec3 CameraForward();
glm::vec3 CameraRight();
