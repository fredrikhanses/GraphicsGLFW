#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

const int MAX_TEXTURES = 4;

struct Material
{
	GLuint program;
	GLuint textures[MAX_TEXTURES];
	int textureNumber = 0;
};

Material LoadMaterial(const char* vertexPath, const char* fragmentPath);
void MaterialAddTexture(Material* material, GLuint texture);

// Helper functions
void MaterialUse(Material material);
void MaterialSet(const char* name, float value);
void MaterialSet(const char* name, const glm::mat4& value);
