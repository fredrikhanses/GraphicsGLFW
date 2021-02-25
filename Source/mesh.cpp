#include "mesh.h"
#include "wavefront.h"
#include "shader.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureCoordinate;
};

Mesh MeshLoad(const char* path)
{
	Wavefront_File* object = wavefront_load(path);
	if (object == nullptr)
	{
		printf("failed to load mesh '%s', file doesn't exist :(\n", path);
		return Mesh();
	}

	Vertex* vertices = new Vertex[object->num_triangles * 3];
	int numberVertices = 0;
	for (int trangleIndex = 0; trangleIndex < object->num_triangles; trangleIndex++)
	{
		Wavefront_Triangle triangle = object->triangles[trangleIndex];
		for (int vertexIndex = 0; vertexIndex < 3; vertexIndex++)
		{
			Wavefront_Vertex& vertex = triangle.vertices[vertexIndex];
			Vertex newVertex;
			newVertex.position = object->positions[vertex.idx_position];
			newVertex.normal = object->normals[vertex.idx_normal];
			newVertex.textureCoordinate = object->texcoords[vertex.idx_texcoord];
			vertices[numberVertices++] = newVertex;
		}
	}
	//Vertex Array Object
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	//Vertex Buffer Object
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * numberVertices, vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), (void*)sizeof(glm::vec3));
	Mesh mesh;
	mesh.vertexArrayObject = vertexArrayObject;
	mesh.drawCount = numberVertices;
	return mesh;
}

void MeshDraw(const Mesh& mesh, const RenderData& data)
{
	glBindVertexArray(mesh.vertexArrayObject);

	//Set material
	MaterialUse(*data.material);

	//Set matrices
	MaterialSet("u_Model", data.model);
	MaterialSet("u_ViewProjection", data.viewProjection);
	
	glDrawArrays(GL_TRIANGLES, 0, mesh.drawCount);
}
