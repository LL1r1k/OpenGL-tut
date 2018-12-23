#include "Game.h"

Game::Game(const char * name, const int width, const int height, bool resize, int GLmajorVer, int GLminorVer)
	:winWidth(width), winHeight(height), GLVerMaj(GLmajorVer), GLVerMin(GLminorVer), title(name), resizable(resize)
{
	fbWidht = width;
	fbHeight = height;

	camPosition = glm::vec3(0.f, 0.f, 1.0f);
	worldUp = glm::vec3(0.f, 1.f, 0.f);
	camFront = glm::vec3(0.f, 0.f, -1.f);

	fov = 90.f;
	nearPlane = 0.1f;
	farPlane = 1000.f;

	initGLFW();
	initWindow();
	initGlew();
	initOpenGLOption();

	initMatrices();
	initShaders();
	initTextures();
	initMaterials();
	initMeshes();
	initLights();
	initUniforms();
}

Game::~Game()
{
	glfwDestroyWindow(window);
	glfwTerminate();

	for (size_t i = 0; i < shaders.size(); i++)
	{
		delete shaders[i];
	}
	for (size_t i = 0; i < textures.size(); i++)
	{
		delete textures[i];
	}
	for (size_t i = 0; i < materials.size(); i++)
	{
		delete materials[i];
	}
	for (size_t i = 0; i < meshes.size(); i++)
	{
		delete meshes[i];
	}
	for (size_t i = 0; i < lights.size(); i++)
	{
		delete lights[i];
	}
}

void Game::update()
{
	glfwPollEvents();
	updateInput(window, *meshes[MESH_QUAD]);
}

void Game::render()
{
	glClearColor(0.f, 0.f, 0.f, 1.f); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); 

	updateUniforms();

	materials[MAT_1]->sendToShader(*shaders[SHADER_CORE_PROGRAM]);

	//Use a program
	shaders[SHADER_CORE_PROGRAM]->use();

	//Activate texture
	textures[TEX_CONTAINER]->bind(0);
	textures[TEX_CONTAINER_SPEC]->bind(1);

	//Render mesh
	meshes[MESH_QUAD]->render(shaders[SHADER_CORE_PROGRAM]);

	glfwSwapBuffers(window);
	glFlush();

	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

int Game::getWindowShouldClose()
{
	return glfwWindowShouldClose(window);
}

void Game::CloseWindow()
{
	glfwSetWindowShouldClose(window, GL_TRUE);
}

void Game::framebuffer_resize_callback(GLFWwindow * window, int fbW, int fbH)
{
	glViewport(0, 0, fbW, fbH);
}

void Game::initWindow()
{
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLVerMaj);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLVerMin);
	glfwWindowHint(GLFW_RESIZABLE, resizable);

	window = glfwCreateWindow(winWidth, winHeight, title, NULL, NULL);

	if (window == nullptr)
	{
		std::cout << "ERROR::GLFW_WINDOW_INIT_FAIELD" << std::endl;
		glfwTerminate();
	}

	glfwGetFramebufferSize(window, &fbWidht, &fbHeight);
	glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);

	glfwMakeContextCurrent(window);
}

void Game::initGLFW()
{
	if (glfwInit() == GL_FALSE)
	{
		std::cout << "ERROR::GLFW_INIT_FAILED" << std::endl;
		glfwTerminate();
	}	
}

void Game::initGlew()
{
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "ERROR::GLEW_INIT_FAILED" << std::endl;
		glfwTerminate();
	}
}

void Game::initOpenGLOption()
{
	glEnable(GL_DEPTH_TEST); // z-coordinate

	glEnable(GL_CULL_FACE); // don't show back face
	glCullFace(GL_BACK); // don't show BACK face
	glFrontFace(GL_CCW); //front face - counter clock wise

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL /*GL_LINE*/); //how to draw polygon fill or just line
}

void Game::initMatrices()
{
	ViewMatrix = glm::mat4(1.f);
	ViewMatrix = glm::lookAt(camPosition, camPosition + camFront, worldUp);

	ProjectionMatrix = glm::mat4(1.f);
	ProjectionMatrix = glm::perspective(glm::radians(fov), static_cast<float>(fbWidht) / fbHeight, nearPlane, farPlane);
}

void Game::initShaders()
{
	shaders.push_back(new Shader("vertex_core.glsl", "fragment_core.glsl"));
}

void Game::initTextures()
{
	textures.push_back(new Texture("Images/mario.png", GL_TEXTURE_2D));
	textures.push_back(new Texture("Images/mario_spec.png", GL_TEXTURE_2D));
	textures.push_back(new Texture("Images/container.png", GL_TEXTURE_2D));
	textures.push_back(new Texture("Images/container_specular.png", GL_TEXTURE_2D));
}

void Game::initMaterials()
{
	materials.push_back(new Material(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), 0, 1));	
}

void Game::initMeshes()
{
	meshes.push_back(new Mesh(&Pyramid()));	
}

void Game::initLights()
{
	lights.push_back(new glm::vec3(0.f, 0.f, 1.f));
}

void Game::initUniforms()
{
	shaders[SHADER_CORE_PROGRAM]->setMat4fv(ViewMatrix, "ViewMatrix");
	shaders[SHADER_CORE_PROGRAM]->setMat4fv(ProjectionMatrix, "ProjectionMatrix");
	shaders[SHADER_CORE_PROGRAM]->setVec3f(*lights[0], "lightPos0");
	shaders[SHADER_CORE_PROGRAM]->setVec3f(camPosition, "cameraPos");
}

void Game::updateInput(GLFWwindow* window, Mesh &mesh)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		mesh.Move(glm::vec3(0.f, 0.f, -0.01f));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		mesh.Move(glm::vec3(0.f, 0.f, 0.01f));
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		mesh.Move(glm::vec3(-0.01f, 0.f, 0.f));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		mesh.Move(glm::vec3(0.01f, 0.f, 0.f));
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		mesh.Rotate(glm::vec3(0.f, -2.f, 0.f));
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		mesh.Rotate(glm::vec3(0.f, 2.f, 0.f));
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		mesh.Scale(glm::vec3(-0.01f));
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	{
		mesh.Scale(glm::vec3(0.01f));
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

void Game::updateUniforms()
{
	glfwGetFramebufferSize(window, &fbWidht, &fbHeight);
	ProjectionMatrix = glm::mat4(1.f);
	ProjectionMatrix = glm::perspective(glm::radians(fov), static_cast<float>(fbWidht) / fbHeight, nearPlane, farPlane);
	shaders[SHADER_CORE_PROGRAM]->setMat4fv(ProjectionMatrix, "ProjectionMatrix");
}
