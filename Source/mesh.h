#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
struct Material;

struct RenderData
{
	glm::mat4 viewProjection = glm::identity<glm::mat4>();
	glm::mat4 model = glm::identity<glm::mat4>();

	Material* material = nullptr;
};

struct Mesh
{
	GLuint vertexArrayObject;
	int drawCount;
};

Mesh MeshLoad(const char* path);

void MeshDraw(const Mesh& mesh, const RenderData& data);