#include <glad/glad.h>		//must be first line!!!
#include <GLFW/glfw3.h>
#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 600;

void resizeCallback(GLFWwindow*, int, int);
void processInput(GLFWwindow*);
void render();

const char* vs_shader = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
void main()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)";

const char* fs_shader = R"(#version 330 core
out vec4 FragColor;
void main()
{
	//FragColor = vec4(0.9f, 0.1f, 0.1f, 1.0f);
	FragColor = vec4(0.1f, 0.6f, 0.1f, 1.0f);
}
)";

//vertex data
const float vertices[] = {
	-0.5f, -0.5f, 0.0f,	//buttom left
	0.5f,  -0.5f, 0.0f, //buttom right
	0.0f,  0.5f,  0.0f  //top
};

const float vertices2[] = {
	0.5f,  0.5f,  0.0f, //top right
	0.5f,  -0.5f, 0.0f,	//buttom right
	-0.5f, -0.5f, 0.0f, //buttom left
	-0.5f,  0.5f,  0.0f  //top left
};

const unsigned int indices[] = {
	0, 1, 3, //first triangle
	1, 2, 3  //second triangle
};

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* win = glfwCreateWindow(WIDTH, HEIGHT, u8"你好,三角形 (F3:线框,F4:实体)", NULL, NULL);
	if (!win)
	{
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(win);
	glfwSetFramebufferSizeCallback(win, resizeCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD\n";
		return -1;
	}

	//build & compile shader program
	int vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vs_shader, NULL);
	glCompileShader(vs);
	//check for shader compile errors
	int success;
	char info[512];
	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vs, 512, NULL, info);
		std::cout << "ERROR::vertex shader compile failed\n" << info << std::endl;
	}
	//build fragment shader
	int fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fs_shader, NULL);
	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fs, 512, NULL, info);
		std::cout << "ERROR::fragment shader compile failed\n" << info << std::endl;
	}
	//link shader
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vs);
	glAttachShader(shaderProgram, fs);
	glLinkProgram(shaderProgram);
	//check for link
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, info);
		std::cout << "ERROR::link shader program failed\n" << info << std::endl;
	}
	glDeleteShader(vs);
	glDeleteShader(fs);

	//set up vertex data (and buffer(s)) and configure vertex attributes
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//bind the Vretex Array Object first, then bind and set vertex buffer(s), and then configure vertex attribute(s)
	glBindVertexArray(VAO);

	//send vertex data to gpu
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind VBO
	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);	//unbind VAO


	while (!glfwWindowShouldClose(win))
	{
		processInput(win);
		render();
		//draw triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(win);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}

void resizeCallback(GLFWwindow* win, int w, int h)
{
	glViewport(0, 0, w, h);
}

void processInput(GLFWwindow* win)
{
	if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		std::cout << "close opengl window\n";
		glfwSetWindowShouldClose(win, true);
	}

	if (glfwGetKey(win, GLFW_KEY_F3) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		std::cout << "F3 -> wireframe mode" << std::endl;
	}

	if (glfwGetKey(win, GLFW_KEY_F4) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		std::cout << "F4 -> polygon mode" << std::endl;
	}
}

void render()
{
	glClearColor(.2f, .3f, .3f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
}