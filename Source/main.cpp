#include <stdio.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/GL.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "mesh.h"
#include "globals.h"
#include "shader.h"

int resolutionX = 1280;
int resolutionY = 720;
int moveRight = 0;
int moveUp = 0;
float scaleMultiplier = 1.0f;

void HandleKeyEvent(GLFWwindow* window, int key, int scancode, int action, int modifiers)
{
	int unitsToMove = 10;
	if (action != GLFW_RELEASE)
	{
		if (key == GLFW_KEY_ESCAPE)
		{
			glfwDestroyWindow(window);
		}
		if (modifiers == GLFW_MOD_ALT && key == GLFW_KEY_ENTER)
		{
			if (glfwGetWindowAttrib(window, GLFW_MAXIMIZED))
			{
				glfwRestoreWindow(window);
			}
			else
			{
				glfwMaximizeWindow(window);
			}
		}
		if (modifiers != GLFW_MOD_ALT && key == GLFW_KEY_LEFT)
		{
			moveRight--;
		}
		if (modifiers != GLFW_MOD_ALT && key == GLFW_KEY_RIGHT)
		{
			moveRight++;
		}
		if (modifiers != GLFW_MOD_ALT && key == GLFW_KEY_UP)
		{
			moveUp++;
		}
		if (modifiers != GLFW_MOD_ALT && key == GLFW_KEY_DOWN)
		{
			moveUp--;
		}
		if (modifiers == GLFW_MOD_ALT && key == GLFW_KEY_LEFT)
		{
			int xPosition;
			int yPosition;
			glfwGetWindowPos(window, &xPosition, &yPosition);
			glfwSetWindowPos(window, xPosition - unitsToMove, yPosition);
		}
		if (modifiers == GLFW_MOD_ALT && key == GLFW_KEY_RIGHT)
		{
			int xPosition;
			int yPosition;
			glfwGetWindowPos(window, &xPosition, &yPosition);
			glfwSetWindowPos(window, xPosition + unitsToMove, yPosition);
		}
		if (modifiers == GLFW_MOD_ALT && key == GLFW_KEY_UP)
		{
			int xPosition;
			int yPosition;
			glfwGetWindowPos(window, &xPosition, &yPosition);
			glfwSetWindowPos(window, xPosition, yPosition - unitsToMove);
		}
		if (modifiers == GLFW_MOD_ALT && key == GLFW_KEY_DOWN)
		{
			int xPosition;
			int yPosition;
			glfwGetWindowPos(window, &xPosition, &yPosition);
			glfwSetWindowPos(window, xPosition, yPosition + unitsToMove);
		}
	}
}

void HandleMouseEvent(GLFWwindow* window, int button, int action, int modifiers)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		double mouseX;
		double mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		printf("MousePosition: \n%f, %f\n", mouseX, mouseY);
		printf("Point: \n%f,%f\n", (mouseX - resolutionX * 0.5f) / (resolutionX * 0.5f), (mouseY - resolutionY * 0.5f) / (-resolutionY * 0.5f));
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if (action == GLFW_PRESS)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			cameraControl = true;
		}
		else
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			cameraControl = false;
		}
	}

}

void HandleMousePosition(GLFWwindow* window, double mouseX, double mouseY)
{
	mousePosition = glm::vec2(mouseX, mouseY);
}

void HandleMouseScroll(GLFWwindow* window, double offsetX, double offsetY)
{
	if (offsetY > 0.0)
	{
		scaleMultiplier += 0.1f;
	}
	else if (offsetY < 0.0)
	{
		scaleMultiplier -= 0.1f;
		if (scaleMultiplier <= 0.0f)
		{
			scaleMultiplier = 0.1f;
		}
	}
}

void HandleFrameBufferResize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	camera.aspect = width / (float)height;
}

GLuint LoadTexture(const char* path)
{
	int width;
	int height;
	int channels;
	stbi_uc* textureData = stbi_load(path, &width, &height, &channels, 0);
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	return texture;
}

int main ()
{
	glfwInit();
	window = glfwCreateWindow(resolutionX, resolutionY, "GraphicsGLFW", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glewInit();

	glfwSetKeyCallback(window, HandleKeyEvent);
	glfwSetMouseButtonCallback(window, HandleMouseEvent);
	glfwSetScrollCallback(window, HandleMouseScroll);
	glfwSetCursorPosCallback(window, HandleMousePosition);

	glfwSetFramebufferSizeCallback(window, HandleFrameBufferResize);

	glEnable(GL_DEPTH_TEST);

	//Load shaders and create program
	Material material = LoadMaterial("Shaders/VertexShader.txt", "Shaders/FragmentShader.txt");
	MaterialAddTexture(&material, LoadTexture("Assets/texture.png"));
	MaterialUse(material);

	Material material2 = LoadMaterial("Shaders/LightingVertexShader.txt", "Shaders/LightingFragmentShader.txt");
	MaterialAddTexture(&material2, LoadTexture("Assets/texture2.jpg"));

	//Mesh load
	Mesh cube = MeshLoad("Assets/Cube.obj");
	Mesh sphere = MeshLoad("Assets/Sphere.obj");
	Mesh monkey = MeshLoad("Assets/Monkey.obj");

	//Aspect
	int width;
	int height;
	glfwGetFramebufferSize(window, &width, &height);
	camera.aspect = width / (float)height;

	//Delta time & mouse delta
	float previousTime = (float)glfwGetTime();
	glm::vec2 lastMousePosition = mousePosition;

	while (!glfwWindowShouldClose(window))
	{
		//Clear screen with grey background
		glClearColor(0.125f, 0.125f, 0.125f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Update Camera
		CameraUpdate();

		//Delta time
		float time = (float)glfwGetTime();
		deltaTime = time - previousTime;
		previousTime = time;

		MaterialSet("u_Time", time);

		//Mouse delta mouse movement
		mouseDelta = mousePosition - lastMousePosition;
		lastMousePosition = mousePosition;
		
		//Set render data
		RenderData renderData;
		renderData.viewProjection = CameraMatrix();
		renderData.material = &material2;
		renderData.directionalLight = normalize(glm::vec3(sin(time), -1.0f, cos(time)));
		renderData.directionalLight = normalize(glm::vec3(-1.0f, -1.0f, -1.0f));

		//Draw cube
		MeshDraw(cube, renderData);

		//Draw sphere
		renderData.model = glm::translate(glm::identity<glm::mat4>(), glm::vec3(2.5f, 0.0f, 0.0f));
		MeshDraw(sphere, renderData);

		//Draw monkey
		renderData.model = glm::translate(glm::identity<glm::mat4>(), glm::vec3(-2.5f, 0.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), time, glm::vec3(0.0f, 1.0f, 0.0f));
		renderData.material = &material2;
		MeshDraw(monkey, renderData);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}