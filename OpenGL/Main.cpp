#include "lib.h"

Vertex vertices[] = {
	//Position						Color						Texcoord				Normal
	glm::vec3(-0.5f, 0.5f, 0.f),	glm::vec3(1.f, 0.f, 0.f),	glm::vec2(0.f, 1.f),	glm::vec3(0.f, 0.f, 1.f),
	glm::vec3(-0.5f, -0.5f, 0.f),	glm::vec3(0.f, 1.f, 0.f),	glm::vec2(0.f, 0.f),	glm::vec3(0.f, 0.f, 1.f),
	glm::vec3(0.5f, -0.5f, 0.f),	glm::vec3(0.f, 0.f, 1.f),	glm::vec2(1.f, 0.f),	glm::vec3(0.f, 0.f, 1.f),
	glm::vec3(0.5f, 0.5f, 0.f),		glm::vec3(1.f, 1.f, 0.f),	glm::vec2(1.f, 1.f),	glm::vec3(0.f, 0.f, 1.f)
};
unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

GLuint indices[]=
{
	0, 1, 2, // Triangle 1
	0, 2, 3 // Trinagle 2
};
unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);


void updateInput(GLFWwindow* window, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		position.z -= 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		position.z += 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		position.x -= 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		position.x += 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		rotation.y -= 2.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		rotation.y += 2.0f;
	}if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		scale -= 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	{
		scale += 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH) 
{
	glViewport(0, 0, fbW, fbH);
}

int main(int args, char** argv)
{
	//--INIT GLFW
	glfwInit();

	//--CREATE WINDOW
	const int WINDOW_WIDHT = 640;
	const int WINDOW_HEIGHT = 640;
	int framebufferWidht = 0;
	int framebufferHeight = 0;

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //OpenGL 4
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5); //.5
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Mac OS

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDHT, WINDOW_HEIGHT, "Title", /*Fullscreen arg*/NULL, NULL);

	glfwGetFramebufferSize(window, &framebufferWidht, &framebufferHeight); // Fow nonResizablw window
	glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback); //For Resizable window
	//glViewport(0, 0, framebufferWidht, framebufferHeight); //Canvas size // Fow nonResizablw window

	glfwMakeContextCurrent(window);

	//--INIT GLEW (NEEDS WINDOW AND OPENGL CONTEXT)
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "ERROR::MAIN.cpp::GLEW_INIT_FAILED" << std::endl;
		glfwTerminate();
	}

	//OPENGL OPTIONS
	glEnable(GL_DEPTH_TEST); // z-coordinate

	glEnable(GL_CULL_FACE); // don't show back face
	glCullFace(GL_BACK); // don't show BACK face
	glFrontFace(GL_CCW); //front face - counter clock wise

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL /*GL_LINE*/); //how to draw polygon fill or just line

	//SHADER INIT	
	Shader core_program("vertex_core.glsl", "fragment_core.glsl");

	//MODEL MESH
	Mesh test(vertices, nrOfVertices, indices, nrOfIndices);
	
	//TEXUTE INIT
		//Texture 0
	Texture texture0("Images/mario.png", GL_TEXTURE_2D);

	//MATERIAL 0
	Material material0(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), texture0.getTextureUint());
	
	//Camera 
	glm::vec3 camPosition(0.f, 0.f, 1.0f);
	glm::vec3 worldUp = glm::vec3(0.f, 1.f, 0.f);
	glm::vec3 camFront = glm::vec3(0.f, 0.f, -1.f);

	//ViewMatrix
	glm::mat4 ViewMatrix(1.f);
	ViewMatrix = glm::lookAt(camPosition, camPosition + camFront, worldUp);

	//ProjectionMatrix
	float fov = 90.f;
	float nearPlane = 0.1f;
	float farPlane = 1000.f;
	glm::mat4 ProjectionMatrix(1.f);
	ProjectionMatrix = glm::perspective(glm::radians(fov), static_cast<float>(framebufferWidht / framebufferHeight), nearPlane, farPlane);

	//Lights
	glm::vec3 lightPos0(0.f, 0.f, 1.f);

	//Unit Uniformss
	core_program.setMat4fv(ViewMatrix, "ViewMatrix");
	core_program.setMat4fv(ProjectionMatrix, "ProjectionMatrix");
	core_program.setVec3f(lightPos0, "lightPos0");
	core_program.setVec3f(camPosition, "cameraPos");
	
	//--MAIN LOOP
	while (!glfwWindowShouldClose(window))
	{
		//UPDATE INPUT
		glfwPollEvents();
		//UPDATE
		//updateInput(window,position, roatation, scale);

		//DRAW
			//CLEAR
		glClearColor(0.f, 0.f, 0.f, 1.f); //RGBA
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); //What u need to clear
		
			//Update uniform
		core_program.set1i(texture0.getTextureUint(), "texture0");
		material0.sendToShader(core_program);

		glfwGetFramebufferSize(window, &framebufferWidht, &framebufferHeight);
		ProjectionMatrix = glm::mat4(1.f);
		ProjectionMatrix = glm::perspective(glm::radians(fov), static_cast<float>(framebufferWidht / framebufferHeight), nearPlane, farPlane);
		core_program.setMat4fv(ProjectionMatrix, "ProjectionMatrix");

			//Use a program
		core_program.use();

			//Activate texture
		texture0.bind();
			//Render test mesh
		test.render(&core_program);

			//END DRAW
		glfwSwapBuffers(window);
		glFlush();
		glBindVertexArray(0);
		glUseProgram(0);
		texture0.unbind();
	}
	//--END OF PROGRAM
	glfwDestroyWindow(window);
	glfwTerminate();

	//DELETE PROGRAM

	return 0;
}
