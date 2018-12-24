#pragma once

#include <iostream>
#include <vector>

#include "Vertex.h"
#include "Primitives.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"

class Mesh
{
public:
	Mesh(Vertex* vertexArray, const unsigned& nrOfVertices, GLuint* indexArray, const unsigned& nrOfIndices,
		glm::vec3& position = glm::vec3(0.f), glm::vec3& rotation = glm::vec3(0.f), glm::vec3& scale = glm::vec3(1.f));
	Mesh(Primitives* primitive,
		glm::vec3& position = glm::vec3(0.f), glm::vec3& rotation = glm::vec3(0.f), glm::vec3& scale = glm::vec3(1.f));
	~Mesh();
	Mesh(const Mesh& mesh);
	void update();
	void render(Shader * shader);
	void updateModelMatrix();

	void setPosition(const glm::vec3& position);
	void setRotation(const glm::vec3& rotation);
	void setOrigin(const glm::vec3& origin);
	void setScale(const glm::vec3& scale);

	void Move(const glm::vec3& position);
	void Rotate(const glm::vec3& rotation);
	void Scale(const glm::vec3& scale);

private:
	Vertex* vertexArray;
	GLuint* indexArray;
	unsigned nrOfVertices, nrOfIndices;

	glm::mat4 ModelMatrix;
	glm::vec3 position;
	glm::vec3 origin;
	glm::vec3 rotation;
	glm::vec3 scale;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	void initVAO();
	void updateUniforms(Shader* shader);

};

