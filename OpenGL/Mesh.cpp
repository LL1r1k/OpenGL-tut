#include "Mesh.h"



Mesh::Mesh(Vertex* vertexArray, const unsigned& nrOfVertices, GLuint* indexArray, const unsigned& nrOfIndices,
	glm::vec3& position, glm::vec3& rotation, glm::vec3& scale)
{
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;

	initVAO(vertexArray, nrOfVertices, indexArray, nrOfIndices);
	updateModelMatrix();
}

Mesh::Mesh(Primitives * primitive, glm::vec3 & position, glm::vec3 & rotation, glm::vec3 & scale)
{
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;

	initVAO(primitive);
	updateModelMatrix();
}


Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Mesh::update()
{

}

void Mesh::render(Shader * shader)
{
	updateModelMatrix();
	updateUniforms(shader);

	shader->use();

	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);
}

void Mesh::updateModelMatrix()
{
	ModelMatrix = glm::mat4(1.f);
	ModelMatrix = glm::translate(ModelMatrix, position);
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
	ModelMatrix = glm::scale(ModelMatrix, scale);
}

void Mesh::setPosition(const glm::vec3 & position)
{
	this->position = position;
}

void Mesh::setRotation(const glm::vec3 & rotation)
{
	this->rotation = rotation;
}

void Mesh::setScale(const glm::vec3 & scale)
{
	this->scale = scale;
}

void Mesh::Move(const glm::vec3 & position)
{
	this->position += position;
}

void Mesh::Rotate(const glm::vec3 & rotation)
{
	this->rotation += rotation;
}

void Mesh::Scale(const glm::vec3 & scale)
{
	this->scale += scale;
}

void Mesh::initVAO(Vertex* vertexArray, const unsigned& nrOfVertices, GLuint* indexArray, const unsigned& nrOfIndices)
{
	this->nrOfVertices = nrOfIndices;
	this->nrOfIndices = nrOfIndices;

	glCreateVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, nrOfVertices * sizeof(Vertex), vertexArray, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, nrOfIndices * sizeof(GLuint), indexArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
}

void Mesh::initVAO(Primitives * primitive)
{
	this->nrOfVertices = primitive->getNrOfVertices();
	this->nrOfIndices = primitive->getNrOfIndices();

	glCreateVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, primitive->getNrOfVertices() * sizeof(Vertex), primitive->getVertices(), GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, primitive->getNrOfIndices() * sizeof(GLuint), primitive->getIndices(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
}

void Mesh::updateUniforms(Shader * shader)
{
	shader->setMat4fv(ModelMatrix, "ModelMatrix");
}
