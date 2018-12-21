#include "lib.h"

Vertex vertices[] = {
	//Position						Color						Texcoord
	glm::vec3(-0.5f, 0.5f, 0.f),	glm::vec3(1.f, 0.f, 0.f),	glm::vec2(0.f, 1.f),
	glm::vec3(-0.5f, -0.5f, 0.f),	glm::vec3(0.f, 1.f, 0.f),	glm::vec2(0.f, 0.f),
	glm::vec3(0.5f, -0.5f, 0.f),	glm::vec3(0.f, 0.f, 1.f),	glm::vec2(1.f, 0.f),
	glm::vec3(0.5f, 0.5f, 0.f),		glm::vec3(1.f, 1.f, 0.f),	glm::vec2(1.f, 1.f)	
};
unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

GLuint indices[]=
{
	0, 1, 2, // Triangle 1
	0, 2, 3 // Trinagle 2
};
unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

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
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL /*GL_LINE*/); //how to draw polygon fill or just line

	//SHADER INIT
	GLuint core_program;
	if (!loadShaders(core_program))
		glfwTerminate();

	//VAO
	GLuint VAO;
	glCreateVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//VBO 
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW/*GL_DYNAMIC_DRAW*/); // dynamic if u want to change vertices

	//EBO
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//SET VERTEXATTRIBPOINTERS AND ENABLE (INPUT ASSEMBLY)
	//Postition
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);
	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);
	//Texcoord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	//TEXUTE INIT
	//Texture 0
	int image_width = 0, image_height = 0;
	unsigned char* image = SOIL_load_image("Images/mario.png", &image_width, &image_height, NULL, SOIL_LOAD_RGBA);

	GLuint texture0;
	glGenTextures(1, &texture0);
	glBindTexture(GL_TEXTURE_2D, texture0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR:TEXTURE_LOADING_FAILED" << std::endl;
	}

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);

	//Texture 1
	int image_width1 = 0, image_height1 = 0;
	unsigned char* image1 = SOIL_load_image("Images/pusheen.png", &image_width1, &image_height1, NULL, SOIL_LOAD_RGBA);

	GLuint texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (image1)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width1, image_height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR:TEXTURE_LOADING_FAILED" << std::endl;
	}

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image1);

	glm::mat4 ModelMatrix(1.f);
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.f, 0.f, 0.f)); // add 
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f)); //x axis
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.f)); 

	glUseProgram(core_program);
	glUniformMatrix4fv(glGetUniformLocation(core_program, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));
	glUseProgram(0);
	
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

			//Use a program
		glUseProgram(core_program);
		
			//Update uniform
		glUniform1i(glGetUniformLocation(core_program, "texture0"), 0);
		glUniform1i(glGetUniformLocation(core_program, "texture1"), 1);

		//Move, rotate and scale 
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.f, 0.f, 0.f)); // add 
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(5.f), glm::vec3(0.f, 0.f, 1.f)); //x axis
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.001f));

		glUniformMatrix4fv(glGetUniformLocation(core_program, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));

			//Activate texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);
			//Bind vertex array object
		glBindVertexArray(VAO);
			
			//DRAW
		//glDrawArrays(GL_TRIANGLES, 0, nrOfVertices);
		glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);


			//END DRAW
		glfwSwapBuffers(window);
		glFlush();
		glBindVertexArray(0);
		glUseProgram(0);
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	//--END OF PROGRAM
	glfwDestroyWindow(window);
	glfwTerminate();

	//DELETE PROGRAM
	glDeleteProgram(core_program);

	return 0;
}
