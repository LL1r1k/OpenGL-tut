#include "Texture.h"

Texture::Texture(const char* fileName, GLenum type)
{	
	this->type = type;
	loadFromFile(fileName);
}

Texture::~Texture()
{
	glDeleteTextures(1, &id);
}

GLuint Texture::GetID() const
{
	return id;
}

void Texture::bind(const GLint textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(type, id);
}

void Texture::unbind()
{
	glActiveTexture(0);
	glBindTexture(type, 0);
}

void Texture::loadFromFile(const char * fileName)
{
	if (id)
		glDeleteTextures(1, &id);

	unsigned char* image = SOIL_load_image(fileName, &width, &height, NULL, SOIL_LOAD_RGBA);

	glGenTextures(1, &id);
	glBindTexture(type, id);

	glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (image)
	{
		glTexImage2D(type, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(type);
	}
	else
	{
		std::cout << "ERROR::TEXTURE::LOADFROMFILE::TEXTURE_LOADING_FAILED: " << fileName << std::endl;
	}

	glActiveTexture(0);
	glBindTexture(type, 0);
	SOIL_free_image_data(image);
}
