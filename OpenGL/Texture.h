#pragma once

#include <iostream>
#include <string>

#include <glew.h> 
#include <glfw3.h> 

#include <SOIL2.h> 

class Texture
{
public:
	Texture(const char* fileName, GLenum type, GLint texture_unit = 0);
	~Texture();
	inline GLuint GetID() const;
	void bind();
	void unbind();
	GLint getTextureUint() const;
	void loadFromFile(const char* fileName);

private:
	GLuint id;
	int height, width;
	unsigned int type;
	GLint textureUnit;
};

