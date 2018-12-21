#include "lib.h"

void updateInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH) 
{
	glViewport(0, 0, fbW, fbH);
}

bool loadShaders(GLuint &program)
{
	bool loadsuccess = true;
	char infolog[512];
	GLint success;

	std::string temp = "", src = "";
	std::ifstream in_file;

	//VERTEX
	in_file.open("vertex_core.glsl");

	if (in_file.is_open())
		while (std::getline(in_file, temp))
			src += temp + "\n";
	else
	{
		std::cout << "ERROR::LOADSHADERS::COULD_NOT_OPEN_VERTEX_CORE" << std::endl;
		loadsuccess = false;
	}

	in_file.close();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vertSrc = src.c_str();
	glShaderSource(vertexShader, 1, &vertSrc, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
		std::cout << "ERROR::LOADSHADERS::COULD_NOT_COMPILE_VERTEX_CORE" << std::endl;
		std::cout << infolog << std::endl;
		loadsuccess = false;
	}

	temp = "";
	src = "";

	//FRAGMENT
	in_file.open("fragment_core.glsl");

	if (in_file.is_open())
		while (std::getline(in_file, temp))
			src += temp + "\n";
	else
	{
		std::cout << "ERROR::LOADSHADERS::COULD_NOT_OPEN_FRAGMENT_CORE" << std::endl;
		loadsuccess = false;
	}

	in_file.close();

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fragSrc = src.c_str();
	glShaderSource(fragmentShader, 1, &fragSrc, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
		std::cout << "ERROR::LOADSHADERS::COULD_NOT_COMPILE_FRAGMENT_CORE" << std::endl;
		std::cout << infolog << std::endl;
		loadsuccess = false;
	}

	//PROGRAM
	program = glCreateProgram();

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infolog);
		std::cout << "ERROR::LOADSHADERS::COULD_NOT_LINK_PROGRAM" << std::endl;
		std::cout << infolog << std::endl;
		loadsuccess = false;
	}

	//END
	glUseProgram(0);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return loadsuccess;
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

	//OPENGL OPTIONS
	glEnable(GL_DEPTH_TEST); // z-coordinate

	glEnable(GL_CULL_FACE); // don't show back face
	glCullFace(GL_BACK); // don't show BACK face
	glFrontFace(GL_CCW); //front face - counter clock wise

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_BLEND_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL /*GL_LINE*/); //how to draw polygon fill or just line

	//SHADER INIT
	GLuint core_program;
	if (!loadShaders(core_program))
		glfwTerminate();

	//--MAIN LOOP
	while (!glfwWindowShouldClose(window))
	{
		//UPDATE INPUT
		glfwPollEvents();
		//UPDATE
		updateInput(window);

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
	glfwDestroyWindow(window);
	glfwTerminate();

	//DELETE PROGRAM
	glDeleteProgram(core_program);

	return 0;
}
