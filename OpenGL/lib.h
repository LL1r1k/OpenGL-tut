#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <glew.h> //graphics
#include <glfw3.h> //window

#include <glm.hpp> //math
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <gtc\matrix_transform.hpp>
#include <mat4x4.hpp>
#include <gtc\type_ptr.hpp>

#include <SOIL2.h> //load image

#include "Shader.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texcoord;
	glm::vec3 normal;
};