#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "camera.h"

extern GLFWwindow* window;

// Time
extern float deltaTime;

// Mouse
extern glm::vec2 mousePosition;
extern glm::vec2 mouseDelta;

// Camera
extern Camera camera;
extern bool cameraControl;
