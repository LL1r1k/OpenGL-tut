#include "Primitives.h"

Primitives::Primitives()
{

}

Primitives::~Primitives()
{

}

void Primitives::set(const Vertex * vertices, const unsigned nrOfVertices, const GLuint * indices, const unsigned nrOfIndices)
{
	for (size_t i = 0; i < nrOfVertices; i++)
		this->vertices.push_back(vertices[i]);

	for (size_t i = 0; i < nrOfIndices; i++)
		this->indices.push_back(indices[i]);
}

Vertex * Primitives::getVertices()
{
	return this->vertices.data();
}

GLuint * Primitives::getIndices()
{
	return this->indices.data();
}

const unsigned Primitives::getNrOfVertices()
{
	return this->vertices.size();
}

const unsigned Primitives::getNrOfIndices()
{
	return this->indices.size();
}


