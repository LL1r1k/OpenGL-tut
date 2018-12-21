#include "lib.h"

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

	glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback); //For Resizable window
	//glfwGetFramebufferSize(window, &framebufferWidht, &framebufferHeight); // Fow nonResizablw window
	//glViewport(0, 0, framebufferWidht, framebufferHeight); //Canvas size // Fow nonResizablw window
	glfwMakeContextCurrent(window);

	//--INIT GLEW (NEEDS WINDOW AND OPENGL CONTEXT)
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "ERROR::MAIN.cpp::GLEW_INIT_FAILED" << std::endl;
		glfwTerminate();
	}

	//--MAIN LOOP
	while (!glfwWindowShouldClose(window))
	{
		//UPDATE INPUT
		glfwPollEvents();
		//UPDATE

		//DRAW
			//CLEAR
		glClearColor(0.f, 0.f, 0.f, 1.f); //RGBA
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); //What u need to clear

			//DRAW

			//END DRAW
		glfwSwapBuffers(window);
		glFlush();
	}
	//--END OF PROGRAM
	glfwTerminate();

	return 0;
}