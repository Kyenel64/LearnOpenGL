#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

const char* vertexShaderSource = 
"#version 460 core\n"
"layout (location = 0) in vec3 aPos;\n" // Takes in vec3
"void main()\n"
"{\n"
	" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n" // gl_Position predefined.
"}\0";

const char* fragmentShaderSource =
"#version 460 core\n"
"out vec4 FragColor;\n" // Takes in vec3
"void main()\n"
"{\n"
" FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n" // gl_Position predefined.
"}\0";

// Our own function to resize viewport.
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	// --- Initializations ----------------------------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Create GLFW window
	GLFWwindow* window = glfwCreateWindow(800, 600, "NewOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// Load glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	// Set opengl context within GLFW window. Origin lower left
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// --- Create Shader Program ----------------------------------------------
	// Compile vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Compile fragment shaders
	unsigned int fragShader;
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragShader);
	// Link shaders into a shader program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);
	glDeleteShader(vertexShader); // Delete shaders once linked.
	glDeleteShader(fragShader);
	glUseProgram(shaderProgram);

	// --- Render Triangle ----------------------------------------------------
	// 2D coordinates for a triangle
	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	// Create VBO Buffer Object
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind VBO to array buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Link data with VBO
	// Specify vertex data format
	// layout location, size of vertex, datatype, isNormalized, vertex spacing, beginning offset.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	// --- Run-loop -----------------------------------------------------------
	while (!glfwWindowShouldClose(window))
	{
		// Process input
		processInput(window);

		// Rendering
		glClearColor(0.3f, 0.3f, 0.4f, 1);
		glClear(GL_COLOR_BUFFER_BIT); // Clears color bits. Can also clear depth

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	


	glfwTerminate();
	return 0;
}