#include <stdio.h>
#include <stdlib.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/GL.h>

void handleKeyEvent(GLFWwindow* window, int key, int scancode, int action, int modifiers)
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
		if (key == GLFW_KEY_LEFT)
		{
			int xPos;
			int yPos;
			glfwGetWindowPos(window, &xPos, &yPos);
			glfwSetWindowPos(window, xPos - unitsToMove, yPos);
		}
		if (key == GLFW_KEY_RIGHT)
		{
			int xPos;
			int yPos;
			glfwGetWindowPos(window, &xPos, &yPos);
			glfwSetWindowPos(window, xPos + unitsToMove, yPos);
		}
		if (key == GLFW_KEY_UP)
		{
			int xPos;
			int yPos;
			glfwGetWindowPos(window, &xPos, &yPos);
			glfwSetWindowPos(window, xPos, yPos - unitsToMove);
		}
		if (key == GLFW_KEY_DOWN)
		{
			int xPos;
			int yPos;
			glfwGetWindowPos(window, &xPos, &yPos);
			glfwSetWindowPos(window, xPos, yPos + unitsToMove);
		}
	}
}

void handleMouseEvent(GLFWwindow* window, int button, int action, int modifiers)
{
	if (action != GLFW_PRESS)
	{
		return;
	}
	if (action == GLFW_MOUSE_BUTTON_LEFT)
	{
		double mouseX;
		double mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
	}
}

int main ()
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(800, 600, "GraphicsGLFW", NULL, NULL);
	glfwMakeContextCurrent(window);
	glewInit();
	glfwSetKeyCallback(window, handleKeyEvent);
	glfwSetMouseButtonCallback(window, handleMouseEvent);
	//VBO
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	float vertexData[] = { 0.0f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f };
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	//VAO
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(1.0f, 0.5f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}