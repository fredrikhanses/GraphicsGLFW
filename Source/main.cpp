#include <stdio.h>
#include <stdlib.h>
#include "TextReader.h"
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/GL.h>

GLuint createShader(const char* source, GLenum type)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	static char LOG_BUFFER[1024];
	glGetShaderInfoLog(shader, 1024, NULL, LOG_BUFFER);
	if (type == 35633)
	{
		printf("-- VERTEX SHADER COMPILE --\n");
	}
	else if(type == 35632)
	{
		printf("-- FRAGMENT SHADER COMPILE --\n");
	}
	printf("Source: \n%s", source);
	printf(LOG_BUFFER);

	return shader;
}

GLuint createShaderProgram(const char* vertexSource, const char* fragmentSource)
{
	GLuint vertex_shader = createShader(vertexSource, GL_VERTEX_SHADER);
	GLuint fragment_shader = createShader(fragmentSource, GL_FRAGMENT_SHADER);

	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	return program;
}

GLuint createVertexArrayBuffer(float* vertexData, int vertexDataSize, GLuint* elementData = nullptr, int elemetDataSize = 0)
{
	//Vertex Buffer Object
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, vertexDataSize, vertexData, GL_STATIC_DRAW);

	//Vertex Array Object
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);

	if (elementData != nullptr && elemetDataSize != 0)
	{
		//Element Buffer Object
		GLuint elementBufferObject;
		glGenBuffers(1, &elementBufferObject);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, elemetDataSize, elementData, GL_STATIC_DRAW);
	}

	return vertexArrayObject;
}

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
			int xPosition;
			int yPosition;
			glfwGetWindowPos(window, &xPosition, &yPosition);
			glfwSetWindowPos(window, xPosition - unitsToMove, yPosition);
		}
		if (key == GLFW_KEY_RIGHT)
		{
			int xPosition;
			int yPosition;
			glfwGetWindowPos(window, &xPosition, &yPosition);
			glfwSetWindowPos(window, xPosition + unitsToMove, yPosition);
		}
		if (key == GLFW_KEY_UP)
		{
			int xPosition;
			int yPosition;
			glfwGetWindowPos(window, &xPosition, &yPosition);
			glfwSetWindowPos(window, xPosition, yPosition - unitsToMove);
		}
		if (key == GLFW_KEY_DOWN)
		{
			int xPosition;
			int yPosition;
			glfwGetWindowPos(window, &xPosition, &yPosition);
			glfwSetWindowPos(window, xPosition, yPosition + unitsToMove);
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
	//Read shaders from file
	TextReader textReader;
	std::string vertexShaderSource = textReader.ReadText("Shaders/VertexShader.txt");
	char* vertexShaderSourceChar = &vertexShaderSource[0];
	std::string fragmentShaderSourceA = textReader.ReadText("Shaders/FragmentShaderA.txt");
	char* fragmentShaderSourceCharA = &fragmentShaderSourceA[0];
	std::string fragmentShaderSourceB = textReader.ReadText("Shaders/FragmentShaderB.txt");
	char* fragmentShaderSourceCharB = &fragmentShaderSourceB[0];
	std::string fragmentShaderSourceC = textReader.ReadText("Shaders/FragmentShaderC.txt");
	char* fragmentShaderSourceCharC = &fragmentShaderSourceC[0];
	std::string fragmentShaderSourceD = textReader.ReadText("Shaders/FragmentShaderD.txt");
	char* fragmentShaderSourceCharD = &fragmentShaderSourceD[0];
	std::string fragmentShaderSourceE = textReader.ReadText("Shaders/FragmentShaderE.txt");
	char* fragmentShaderSourceCharE = &fragmentShaderSourceE[0];

	glfwInit();
	GLFWwindow* window = glfwCreateWindow(800, 800, "GraphicsGLFW", NULL, NULL);
	glfwMakeContextCurrent(window);

	glewInit();

	glfwSetKeyCallback(window, handleKeyEvent);
	glfwSetMouseButtonCallback(window, handleMouseEvent);

	//Program A
	float vertexDataA[] =
	{ 
		-0.9, 0.7, 
		-0.9f, 0.9f, 
		-0.1f, 0.9f, 
		-0.1f, 0.7f 
	};
	GLuint elementDataA[] = { 0, 1, 2, 0, 2, 3 };
	GLuint triA = createVertexArrayBuffer(vertexDataA, sizeof(vertexDataA), elementDataA, sizeof(elementDataA));
	GLuint programA = createShaderProgram(vertexShaderSourceChar, fragmentShaderSourceCharA);

	//Program B
	float vertexDataB[] = 
	{ 
		0.5f, 0.5f, 
		0.5f, 0.9f, 
		0.9f, 0.9f, 
		0.9f, 0.5f 
	};
	GLuint elementDataB[] = { 0, 1, 2, 0, 2, 3 };
	GLuint triB = createVertexArrayBuffer(vertexDataB, sizeof(vertexDataB), elementDataB, sizeof(elementDataB));
	GLuint programB = createShaderProgram(vertexShaderSourceChar, fragmentShaderSourceCharB);

	//Program C
	float vertexDataC[] =
	{ 
		-0.9f, -0.4f, 
		-0.9f, 0.2f, 
		-0.7f, 0.2f, 
		-0.7f, -0.4f, 
		-0.7f, -0.2f, 
		-0.5f, -0.2f, 
		-0.5f, -0.4f 
	};
	GLuint elementDataC[] = { 0, 1, 2, 0, 2, 3, 3, 4, 5, 3, 5, 6 };
	GLuint triC = createVertexArrayBuffer(vertexDataC, sizeof(vertexDataC), elementDataC, sizeof(elementDataC));
	GLuint programC = createShaderProgram(vertexShaderSourceChar, fragmentShaderSourceCharC);

	//Program D
	float vertexDataD[] =
	{ 
		0.2f, -0.4f,
		0.2f, 0.0f,
		0.4f, 0.0f,
		0.4f, -0.4f,
		0.4f, -0.2f,
		0.6f, -0.2f,
		0.6f, -0.6f,
		0.4f, -0.6f 
	};
	GLuint elementDataD[] = { 0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7 };
	GLuint triD = createVertexArrayBuffer(vertexDataD, sizeof(vertexDataD), elementDataD, sizeof(elementDataD));
	GLuint programD = createShaderProgram(vertexShaderSourceChar, fragmentShaderSourceCharD);

	//Program E
	float vertexDataE[] = 
	{ 
		-0.1f, 0.0f,
		-0.3f, 0.0f,
		-0.3f, 0.2f,
		-0.1f, 0.2f,
		-0.5f, 0.2f,
		-0.5f, 0.4f,
		0.1f, 0.4f,
		0.1f, 0.2f
	};
	GLuint elementDataE[] = { 0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7 };
	GLuint triE = createVertexArrayBuffer(vertexDataE, sizeof(vertexDataE), elementDataE, sizeof(elementDataE));
	GLuint programE = createShaderProgram(vertexShaderSourceChar, fragmentShaderSourceCharE);

	while (!glfwWindowShouldClose(window))
	{
		//Clear screen
		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(programA);
		glBindVertexArray(triA);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glUseProgram(programB);
		glBindVertexArray(triB);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glUseProgram(programC);
		glBindVertexArray(triC);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

		glUseProgram(programD);
		glBindVertexArray(triD);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

		glUseProgram(programE);
		glBindVertexArray(triE);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}