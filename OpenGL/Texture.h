#pragma once

#include <iostream>
#include <string>

#include <glew.h> 
#include <glfw3.h> 

#include <SOIL2.h> 

class Texture
{
public:
	Texture(const char* fileName, GLenum type);
	~Texture();
	inline GLuint GetID() const;
	void bind(const GLint textureUnit);
	void unbind();
	void loadFromFile(const char* fileName);

private:
	GLuint id;
	int height, width;
	unsigned int type;
};

