#pragma once

#include <iostream>
#include <vector>

#include "Vertex.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"

class Mesh
{
public:
	Mesh(Vertex* vertexArray, const unsigned& nrOfVertices, GLuint* indexArray, const unsigned& nrOfIndices);
	~Mesh();
	void update();
	void render(Shader * shader);

private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	glm::mat4 ModelMatrix;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	void initVertexData(Vertex* vertexArray, const unsigned& nrOfVertices, GLuint* indexArray, const unsigned& nrOfIndices);
	void initVAO();
	void initModelMatrix();
	void updateUniforms(Shader* shader);

};

