#pragma once

#include "lib.h"

enum shader_enums {
	SHADER_CORE_PROGRAM = 0
};
enum texture_enum {
	TEX_MARIO = 0,
	TEX_MARIO_SPEC = 1,
	TEX_CONTAINER = 2,
	TEX_CONTAINER_SPEC = 3
};
enum material_enum {
	MAT_1 = 0
};
enum mesh_enum {
	MESH_QUAD = 0
};

class Game
{
public:
	Game(const char * name, const int width = 640, const int height = 640, bool resize = true , int GLmajorVer = 4, int GLminorVer = 5);
	virtual ~Game();

	void update();
	void render();

	int getWindowShouldClose();

	void CloseWindow();

	static void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH);

private:
	void initWindow();
	void initGLFW();
	void initGlew();
	void initOpenGLOption();
	void initMatrices();
	void initShaders();
	void initTextures();
	void initMaterials();
	void initMeshes();
	void initLights();
	void initUniforms();

	void updateInput(GLFWwindow* window, Mesh &mesh);
	void updateUniforms();

	//Window
	GLFWwindow* window;
	const char* title;
	const int winWidth, winHeight;
	const bool resizable;
	int fbWidht, fbHeight;

	//OpenGL
	const int GLVerMaj, GLVerMin;

	//Matrices
	glm::mat4 ViewMatrix, ProjectionMatrix;
	glm::vec3 camPosition, worldUp, camFront;
	float fov, nearPlane, farPlane;

	//Shaders
	std::vector<Shader*> shaders;

	//Textures
	std::vector<Texture*> textures;

	//Material
	std::vector<Material*> materials;

	//Meshes
	std::vector<Mesh*> meshes;

	//Lights
	std::vector<glm::vec3*> lights;
	
};

