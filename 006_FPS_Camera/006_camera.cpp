#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.h>
#include <filesystem.h>
#include <iostream>
using namespace std;

void on_size_changed(GLFWwindow*, int, int);
void processInput(GLFWwindow*);
void on_mouse_pos_changed(GLFWwindow*, double, double);
void scroll_callback(GLFWwindow*, double, double);

glm::vec3 camPos = glm::vec3(0.f, 0.f, 3.f);
glm::vec3 camFront = glm::vec3(0.f, 0.f, -1.f);
glm::vec3 camUp = glm::vec3(0.f, 1.f, 0.f);
float deltaTime = 0.f;
float lastFrameTime = 0.f;

//mouse control XZ pitch, yaw
bool firstMouse = true;
float lastX = 400;
float lastY = 300;
float pitch = 0.f;
float yaw = -90.f;
float fov = 45.f;

//mouse wheel


int main()
{
	const unsigned int WIDTH = 800;
	const unsigned int HEIGHT = 600;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* win = glfwCreateWindow(WIDTH, HEIGHT, u8"FPS Camera(ADWS to move, Mouse to rotate)", NULL, NULL);
	if (win == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		return -1;
	}
	glfwMakeContextCurrent(win);
	glfwSetFramebufferSizeCallback(win, on_size_changed);

	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(win, on_mouse_pos_changed);
	glfwSetScrollCallback(win, scroll_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	//10 boxes pos
	glm::vec3 posList[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glEnable(GL_DEPTH_TEST);
	Shader shader = Shader(FileSystem::getPath("shaders/006_camera.vs").c_str(), FileSystem::getPath("shaders/006_camera.fs").c_str());

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//position, uv coord attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* pixels = stbi_load(FileSystem::getPath("textures/wall.jpg").c_str(), &width, &height, &channels, 0);
	if (pixels)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Failed to load texture" << endl;
	}
	stbi_image_free(pixels);

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	pixels = stbi_load(FileSystem::getPath("textures/duck.bmp").c_str(), &width, &height, &channels, 0);
	if (pixels)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Failed to load texture" << endl;
	}
	stbi_image_free(pixels);

	//which texture unit belongs to sampler? (only once)
	shader.use();
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);

	//render loop
	while (!glfwWindowShouldClose(win))
	{
		float currentFrameTime = glfwGetTime();
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		processInput(win);

		glClearColor(.2f, .3f, .3f, 1.f);
		//also clear depth buffer now
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		//activate shader
		shader.use();
		//create transformations
		glm::mat4 view = glm::lookAt(camPos, camPos + camFront, camUp);
		shader.setMat4("view", view);
		//glm::mat4 projection = glm::perspective(glm::radians(45.f), float(WIDTH) / float(HEIGHT), .1f, 100.f);
		glm::mat4 projection = glm::perspective(glm::radians(fov), float(WIDTH) / float(HEIGHT), .1f, 100.f);
		shader.setMat4("projection", projection);

		//render the box
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36); //6 * 6

		//10 boxes
		for (size_t i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.f);
			model = glm::translate(model, posList[i]);
			float angle = 20.f * i;
			if (i % 3 == 0)
			{
				angle = 20.f * glfwGetTime();
			}
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.f, .3f, .5f));
			shader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}

void on_size_changed(GLFWwindow* win, int w, int h)
{
	glViewport(0, 0, w, h);
}

void processInput(GLFWwindow* win)
{
	if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(win, true);
	}

	//move camera
	float speed = 2.f * deltaTime;
	if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
	{
		camPos -= glm::normalize(glm::cross(camFront, camUp)) * speed;
	}

	if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
	{
		camPos += glm::normalize(glm::cross(camFront, camUp)) * speed;
	}

	if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
	{
		camPos += camFront * speed;
	}

	if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
	{
		camPos -= camFront * speed;
	}

}

void on_mouse_pos_changed(GLFWwindow*, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = .01f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.f)
	{
		pitch = 89.f;
	}
	if (pitch < -89.f)
	{
		pitch = -89.f;
	}

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}