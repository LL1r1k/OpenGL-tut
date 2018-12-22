#pragma once

#include <iostream>

#include <glew.h>
#include <glfw3.h> 

#include <glm.hpp> 
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include <gtc\type_ptr.hpp>

#include "Shader.h"

using namespace glm;

class Material
{
public:
	Material(vec3 ambient, vec3 diffuse,vec3 specular, GLint diffuseTex = NULL, GLint specularTex = NULL);
	~Material();
	void sendToShader(Shader& programID);

private:
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	GLint diffuseTex;
	GLint specularTex;

};

