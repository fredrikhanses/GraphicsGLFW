#include "camera.h"
#include <glm/gtc/quaternion.hpp>
#include "globals.h"

void CameraUpdate()
{
	if (!cameraControl)
	{
		return;
	}

	// Rotation
	camera.yaw -= mouseDelta.x * CAM_SENSITIVITY;
	camera.pitch -= mouseDelta.y * CAM_SENSITIVITY;

	// Movement
	glm::vec3 move = glm::vec3(0.0f);
	if (glfwGetKey(window, GLFW_KEY_D))
	{
		move += CameraRight();
	}
	if (glfwGetKey(window, GLFW_KEY_A))
	{
		move -= CameraRight();
	}
	if (glfwGetKey(window, GLFW_KEY_W))
	{
		move += CameraForward();
	}
	if (glfwGetKey(window, GLFW_KEY_S))
	{
		move -= CameraForward();
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE))
	{
		move += glm::vec3(0.0f, 1.0f, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))
	{
		move -= glm::vec3(0.0f, 1.0f, 0.0f);
	}
	camera.position += move * CAM_SPEED * deltaTime;
}

glm::mat4 CameraMatrix()
{
	glm::mat4 viewMatrix = glm::lookAt(camera.position, camera.position + CameraForward(), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 projectionMatrix;
	if (camera.perspective)
	{
		projectionMatrix = glm::perspective(glm::radians(camera.fov), camera.aspect, camera.near, camera.far);
	}
	else
	{
		float halfHeight = camera.orthoHeight * 0.5f;
		projectionMatrix = glm::ortho(-halfHeight * camera.aspect, halfHeight * camera.aspect, -halfHeight, halfHeight, camera.near, camera.far);
	}
	return projectionMatrix * viewMatrix;
}

glm::quat CameraQuaternion()
{
	return glm::angleAxis(camera.yaw, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::angleAxis(camera.pitch, glm::vec3(1.0f, 0.0f, 0.0f));
}

glm::vec3 CameraForward()
{
	return CameraQuaternion() * glm::vec3(0.0f, 0.0f, -1.0f);
}

glm::vec3 CameraRight()
{
	return CameraQuaternion() * glm::vec3(1.0f, 0.0f, 0.0f);
}

