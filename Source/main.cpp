#include <stdio.h>
#include <stdlib.h>
#include "TextReader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/GL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int resolutionX = 1000;
int resolutionY = 1000;
int moveRight = 0;
int moveUp = 0;
float scaleMultiplier = 1.0f;
bool mouseCamera = false;

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

GLuint createVertexArrayBuffer(float* vertexData, int vertexDataSize, GLuint* elementData, int elemetDataSize)
{
	//Vertex Array Object
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	//Vertex Buffer Object
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, vertexDataSize, vertexData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 7 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 7 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, 7 * sizeof(float), (void*)(5 * sizeof(float)));

	//Element Buffer Object
	GLuint elementBufferObject;
	glGenBuffers(1, &elementBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elemetDataSize, elementData, GL_STATIC_DRAW);

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

void handleMouseEvent(GLFWwindow* window, int button, int action, int modifiers)
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
			mouseCamera = true;
		}
		else
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			mouseCamera = false;
		}
	}

}

void handleMouseScroll(GLFWwindow* window, double offsetX, double offsetY)
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

void handleFrameBufferResize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

GLuint loadTexture(const char* path)
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
	//Read shaders from file
	TextReader textReader;
	std::string vertexShaderSource = textReader.ReadText("Shaders/VertexShader.txt");
	char* vertexShaderSourceChar = &vertexShaderSource[0];
	std::string fragmentShaderSource = textReader.ReadText("Shaders/FragmentShader.txt");
	char* fragmentShaderSourceChar = &fragmentShaderSource[0];

	glfwInit();
	GLFWwindow* window = glfwCreateWindow(resolutionX, resolutionY, "GraphicsGLFW", NULL, NULL);
	glfwMakeContextCurrent(window);

	glewInit();

	glfwSetKeyCallback(window, handleKeyEvent);
	glfwSetMouseButtonCallback(window, handleMouseEvent);
	glfwSetScrollCallback(window, handleMouseScroll);

	glfwSetFramebufferSizeCallback(window, handleFrameBufferResize);

	glEnable(GL_DEPTH_TEST);

	GLuint program = createShaderProgram(vertexShaderSourceChar, fragmentShaderSourceChar);
	GLuint uTime = glGetUniformLocation(program, "u_Time");
	GLuint uZoom = glGetUniformLocation(program, "u_Zoom");
	GLuint uColour = glGetUniformLocation(program, "u_Colour");
	GLuint uOffset = glGetUniformLocation(program, "u_Offset");
	GLuint uMidPoint = glGetUniformLocation(program, "u_MidPoint");
	GLuint uSampler1 = glGetUniformLocation(program, "u_Sampler1");
	GLuint uModel = glGetUniformLocation(program, "u_Model");
	GLuint uViewProjection = glGetUniformLocation(program, "u_ViewProjection");

	//Program E
	float vertexDataE[] = 
	{
		//Position		//Colour			//UV
		0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
		-0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f,
		-0.5f, 0.5f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,
		0.5f, 0.5f,		0.5f, 0.0f, 0.0f,	1.0f, 0.0f
	};
	GLuint elementDataE[] = { 0, 1, 2, 0, 2, 3 };
	GLuint programE = createVertexArrayBuffer(vertexDataE, sizeof(vertexDataE), elementDataE, sizeof(elementDataE));
	
	glUseProgram(program);

	GLuint texture0 = loadTexture("Textures/texture.png");
	glActiveTexture(GL_TEXTURE1);
	GLuint texture1 = loadTexture("Textures/texture2.jpg");

	float previousTime = (float)glfwGetTime();

	double lastMouseX;
	double lastMouseY;
	glfwGetCursorPos(window, &lastMouseX, &lastMouseY);

	glBindVertexArray(programE);

	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraRotation = glm::vec3();

	while (!glfwWindowShouldClose(window))
	{
		//Clear screen with grey background
		glClearColor(0.125f, 0.125f, 0.125f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float time = (float)glfwGetTime();
		float deltaTime = time - previousTime;
		previousTime = time;

		double mouseX;
		double mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		float deltaMouseX = lastMouseX - mouseX;
		float deltaMouseY = lastMouseY - mouseY;
		lastMouseX = mouseX;
		lastMouseY = mouseY;

		glUniform1f(uTime, time);
		glUniform1i(uSampler1, 1);
		glUniform2f(uMidPoint, 0.0f, 0.0f);
		glUniform2f(uOffset, moveRight * 0.05f, moveUp * 0.05f);
		glUniform1f(uZoom, scaleMultiplier);

		//Rotate Camera
		if (mouseCamera)
		{
			glm::vec3 cameraRot = glm::vec3(deltaMouseY, deltaMouseX, 0.0f);
			cameraRotation += cameraRot * glm::radians(0.1f);
		}

		glm::quat cameraQuat = angleAxis(cameraRotation.y, glm::vec3(0.0f, 1.0f, 0.0f)) * angleAxis(cameraRotation.x, glm::vec3(1.0f, 0.0f, 0.0f)) * angleAxis(cameraRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::vec3 cameraForward = cameraQuat * glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 cameraRight = cameraQuat * glm::vec3(1.0f, 0.0f, 0.0f);

		//Move Camera
		glm::vec3 cameraMove = glm::vec3();
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			cameraMove -= cameraRight;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			cameraMove += cameraRight;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			cameraMove -= cameraForward;
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			cameraMove += cameraForward;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		{
			cameraMove.y -= 1.0f;
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			cameraMove.y += 1.0f;
		}

		cameraPosition += cameraMove * deltaTime;

		int width;
		int height;
		glfwGetWindowSize(window, &width, &height);
		float aspect = (float)width / height;

		//Projection, View
		glm::mat4 viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraForward, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 projectionMatrix = glm::perspective(glm::radians(50.0f), aspect, 0.5f, 10.0f);
		glm::mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;
		glUniformMatrix4fv(uViewProjection, 1, false, glm::value_ptr(viewProjectionMatrix));

		//(Translation, Rotation, Scale) = Model
		glm::mat4 translationMatrix = glm::translate(glm::identity<glm::mat4>(), glm::vec3(0.0f, -0.5f, 0.0f));
		glm::mat4 scaleMatrix = glm::scale(glm::identity<glm::mat4>(), glm::vec3(1.5f));
		glm::mat4 rotationMatrix = glm::rotate(glm::identity<glm::mat4>(), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
		glUniformMatrix4fv(uModel, 1, false, glm::value_ptr(modelMatrix));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		translationMatrix = glm::translate(glm::identity<glm::mat4>(), glm::vec3(0.0f));
		scaleMatrix = glm::scale(glm::identity<glm::mat4>(), glm::vec3(1.0f));
		rotationMatrix = glm::rotate(glm::identity<glm::mat4>(), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
		glUniformMatrix4fv(uModel, 1, false, glm::value_ptr(modelMatrix));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}