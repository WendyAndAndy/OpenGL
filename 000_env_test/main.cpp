#include <glad/glad.h>		//must be first line!!!
#include <GLFW/glfw3.h>
#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 600;

void resizeCallback(GLFWwindow*, int, int);
void processInput(GLFWwindow*);
void render();

int main()
{
	std::cout << "opengl dev-env testing..\n按任意键开始..." << std::endl;
	getchar();

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* win = glfwCreateWindow(WIDTH, HEIGHT, u8"你好,OpenGL", NULL, NULL);
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

	while (!glfwWindowShouldClose(win))
	{
		processInput(win);
		render();
		glfwSwapBuffers(win);
		glfwPollEvents();
	}

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
}

void render()
{
	glClearColor(.2f, .3f, .3f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
}