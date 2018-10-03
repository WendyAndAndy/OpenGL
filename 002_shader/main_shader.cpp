#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include <filesystem.h>

using namespace std;

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;
void framebuffer_size_callback(GLFWwindow*, int, int);
void processInput(GLFWwindow*);

const float vertices[] = {
	.5f, -.5f, 0.0f, 1.0f, 0.0f, 0.0f,	//bottom right, red
	-.5f,-.5f, 0.0f, 0.0f, 1.0f, 0.0f,  //bottom left, green
	0.0f, .5f, 0.0f, 0.0f, 0.0f, 1.0f	//top middle, blue
};

Shader shader;
float xPos = 0.0f;	//for vertex-shader move the triangle
float yPos = 0.0f;	//for vertex-shader move the triangle


int main()
{
	/*cout << "003 hello shader" << endl;
	getchar();*/
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, u8"ÄãºÃ, Shader(WSAD move..)", NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD\n";
	}

	shader = Shader(FileSystem::getPath("shaders/002_shader.vs").c_str(), FileSystem::getPath("shaders/002_shader.fs").c_str());

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		//glClearColor(.2, .3, .3, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		cout << "F3 -> wireframe mode" << endl;
	}

	if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		cout << "F4 -> polygon mode" << endl;
	}

	const float SPEED = 0.001f;
	//Move Left
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		xPos -= SPEED;
		shader.setFloat("xPos", xPos);
		cout << "pressed A key, xPos = " << xPos << endl;
	}
	//Move Right
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		xPos += SPEED;
		shader.setFloat("xPos", xPos);
		cout << "pressed D key, xPos = " << xPos << endl;
	}
	//Move Up
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		yPos += SPEED;
		shader.setFloat("yPos", yPos);
		cout << "pressed W key, yPos = " << yPos << endl;
	}
	//Move Down
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		yPos -= SPEED;
		shader.setFloat("yPos", yPos);
		cout << "pressed S key, yPos = " << yPos << endl;
	}

}