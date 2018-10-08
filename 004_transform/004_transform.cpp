#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader.h>
#include <stb_image.h>
#include <filesystem.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
using namespace std;

const int WIDTH = 800;
const int HEIGHT = 600;
void on_size_change(GLFWwindow*, int, int);
void processInput(GLFWwindow*);

//for mix
float mixValue = 0.1f;

int main()
{
	glm::vec4 vec(1, 0, 0, 1);
	cout << vec.x << ", " << vec.y << ", " << vec.z << endl;
	glm::mat4 trans = glm::mat4(1); //unit matrix
	//construct move matrix
	trans = glm::translate(trans, glm::vec3(1, 1, 0));
	//move
	vec = trans * vec;
	cout << vec.x << ", " << vec.y << ", " << vec.z << endl;
	//getchar();

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 8);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, u8"Matrix, 应用矩阵进行变换", NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, on_size_change);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	Shader shader = Shader(FileSystem::getPath("shaders/004_transform.vs").c_str(), FileSystem::getPath("shaders/004_transform.fs").c_str());

	float vertices[] = {
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	//glBindBuffer(GL_VERTEX_ARRAY, VBO);
	//glBufferData(GL_VERTEX_ARRAY, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//load texture
	unsigned int texture1;
	//glGenBuffers(1, &texture1);
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(FileSystem::getPath("textures/wall.jpg").c_str(), &width, &height, &channels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Failed to load texture1" << endl;
	}
	stbi_image_free(data);

	//load texture2
	unsigned int texture2;
	//glGenBuffers(1, &texture2);
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load(FileSystem::getPath("textures/awesomeface.png").c_str(), &width, &height, &channels, 0);
	if (data)
	{

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Failed to load texture2" << endl;
	}
	stbi_image_free(data);

	//for mix
	shader.use();
	glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0);
	shader.setInt("texture2", 1);

	//Matrix * Vector
	//T * R * S * V
	//trans = glm::mat4(1);
	//trans = glm::rotate(trans, glm::radians(45.f), glm::vec3(0, 0, 1));
	//trans = glm::scale(trans, glm::vec3(.5, .5, .5));
	unsigned int transLoc = glGetUniformLocation(shader.ID, "trans");
	//glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(trans));

	//main loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//glBindTexture(GL_TEXTURE_2D, texture);

		//shader.use();
		//glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0);
		//shader.setInt("texture2", 1);

		//for mix
		//bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		//rotate by time
		trans = glm::mat4(1.f);
		trans = glm::translate(trans, glm::vec3(.5f, -.5f, 0));
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0, 0, 1));
		glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(trans));

		shader.setFloat("mixValue", mixValue);
		shader.use();

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//second
		trans = glm::mat4(1.f);
		trans = glm::translate(trans, glm::vec3(-.5f, .5f, 0));
		//float scale = abs(cos(glfwGetTime()));
		float scale = sin(glfwGetTime());
		trans = glm::scale(trans, glm::vec3(scale, scale, scale));
		glUniformMatrix4fv(transLoc, 1, GL_FALSE, &trans[0][0]); //take the matrix value array's first element as its memory pointer value

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}

void on_size_change(GLFWwindow* window, int w, int h)
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

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		std::cout << "mixValue += 0.001 = " << mixValue << std::endl;
		mixValue += 0.001f; // change this value accordingly (might be too slow or too fast based on system hardware)
		if (mixValue >= 1.0f)
			mixValue = 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		std::cout << "mixValue -= 0.001 = " << mixValue << std::endl;
		mixValue -= 0.001f; // change this value accordingly (might be too slow or too fast based on system hardware)
		if (mixValue <= 0.0f)
			mixValue = 0.0f;
	}
}