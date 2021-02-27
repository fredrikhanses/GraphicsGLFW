#include "shader.h"
#include <stdio.h>
#include <glm/gtc/type_ptr.hpp>

GLuint LoadShader(const char* path, GLenum type)
{
	printf("Loading shader '%s'\n", path);

	// Load the file!
	FILE* file;
	fopen_s(&file, path, "r");

	static char BUFFER[2048];
	int file_size = fread(BUFFER, 1, 2048, file);
	fclose(file);

	BUFFER[file_size] = 0;

	// Create the shader OpenGL object
	char* bufferPointer = BUFFER;

	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &bufferPointer, NULL);
	glCompileShader(shader);

	// Print result of compilation
	glGetShaderInfoLog(shader, 1024, NULL, BUFFER);
	printf(BUFFER);

	return shader;
}

GLuint LoadShaderProgram(const char* vertexPath, const char* fragmentPath)
{
	GLuint vertexShader = LoadShader(vertexPath, GL_VERTEX_SHADER);
	GLuint fragmentShader = LoadShader(fragmentPath, GL_FRAGMENT_SHADER);

	// Create the program!
	GLuint program = glCreateProgram();

	// Attach and link shaders
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	static char BUFFER[2048];
	glGetProgramInfoLog(program, 2048, NULL, BUFFER);
	printf(BUFFER);

	return program;
}

Material LoadMaterial(const char* vertexPath, const char* fragmentPath)
{
	Material material;
	material.program = LoadShaderProgram(vertexPath, fragmentPath);

	return material;
}

void MaterialAddTexture(Material* material, GLuint texture)
{
	material->textures[material->textureNumber++] = texture;
}

// Helper functions
static GLuint currentProgram;

void MaterialUse(Material material)
{
	glUseProgram(material.program);

	for(int i = 0; i<material.textureNumber; ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, material.textures[i]);
	}

	currentProgram = material.program;
}

void MaterialSet(const char* name, float value)
{
	GLuint location = glGetUniformLocation(currentProgram, name);
	glUniform1f(location, value);
}

void MaterialSet(const char* name, const glm::vec3& value)
{
	GLuint location = glGetUniformLocation(currentProgram, name);
	glUniform3fv(location, 1, value_ptr(value));
}

void MaterialSet(const char* name, const glm::mat4& value)
{
	GLuint location = glGetUniformLocation(currentProgram, name);
	glUniformMatrix4fv(location, 1, false, value_ptr(value));
}
