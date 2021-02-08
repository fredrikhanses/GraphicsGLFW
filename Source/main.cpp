#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>

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
	GLFWwindow* window = glfwCreateWindow(600, 800, "GraphicsGLFW", NULL, NULL);
	glfwSetKeyCallback(window, handleKeyEvent);
	glfwSetMouseButtonCallback(window, handleMouseEvent);
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		
	}
	return 0;
}