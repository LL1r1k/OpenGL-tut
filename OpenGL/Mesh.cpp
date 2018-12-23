#include "Mesh.h"



Mesh::Mesh(Vertex* vertexArray, const unsigned& nrOfVertices, GLuint* indexArray, const unsigned& nrOfIndices,
	glm::vec3& position, glm::vec3& rotation, glm::vec3& scale)
{
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;

	this->nrOfIndices = nrOfIndices;
	this->nrOfVertices = nrOfVertices;

	this->vertexArray = new Vertex[nrOfVertices];
	for (size_t i = 0; i < nrOfVertices; i++)
	{
		this->vertexArray[i] = vertexArray[i];
	}
	this->indexArray = new GLuint[nrOfIndices];
	for (size_t i = 0; i < nrOfIndices; i++)
	{
		this->indexArray[i] = indexArray[i];
	}

	initVAO();
	updateModelMatrix();
}

Mesh::Mesh(Primitives * primitive, glm::vec3 & position, glm::vec3 & rotation, glm::vec3 & scale)
{
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;

	this->nrOfIndices = primitive->getNrOfIndices();
	this->nrOfVertices = primitive->getNrOfVertices();

	this->vertexArray = new Vertex[nrOfVertices];
	for (size_t i = 0; i < nrOfVertices; i++)
	{
		this->vertexArray[i] = primitive->getVertices()[i];
	}
	this->indexArray = new GLuint[nrOfIndices];
	for (size_t i = 0; i < nrOfIndices; i++)
	{
		this->indexArray[i] = primitive->getIndices()[i];
	}

	initVAO();
	updateModelMatrix();
}


Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	if(nrOfIndices > 0)
		glDeleteBuffers(1, &EBO);
	delete[] vertexArray;
	delete[] indexArray;
}

Mesh::Mesh(const Mesh & obj)
{
	this->position = obj.position;
	this->rotation = obj.rotation;
	this->scale = obj.scale;

	this->nrOfIndices = obj.nrOfIndices;
	this->nrOfVertices = obj.nrOfVertices;

	this->vertexArray = new Vertex[nrOfVertices];
	for (size_t i = 0; i < nrOfVertices; i++)
	{
		this->vertexArray[i] = obj.vertexArray[i];
	}
	this->indexArray = new GLuint[nrOfIndices];
	for (size_t i = 0; i < nrOfIndices; i++)
	{
		this->indexArray[i] = obj.indexArray[i];
	}

	initVAO();
	updateModelMatrix();
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

	if (nrOfIndices > 0)
		glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, nrOfVertices);

	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Mesh::updateModelMatrix()
{
	ModelMatrix = glm::mat4(1.f);
	ModelMatrix = glm::translate(ModelMatrix, origin);
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
	ModelMatrix = glm::translate(ModelMatrix, position - origin);
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

void Mesh::setOrigin(const glm::vec3 & origin)
{
	this->origin = origin;
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

void Mesh::initVAO()
{
	glCreateVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, nrOfVertices * sizeof(Vertex), vertexArray, GL_STATIC_DRAW);

	if (nrOfIndices > 0)
	{
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, nrOfIndices * sizeof(GLuint), indexArray, GL_STATIC_DRAW);
	}	

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
