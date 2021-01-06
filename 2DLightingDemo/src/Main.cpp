#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

static int w_width = 900, w_height = 900;

int main()
{
	if (!glfwInit())
	{
		std::cout << "Error initializing glfw" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(w_width, w_height, "2D Lighting demo!", NULL, NULL);
	if (!window)
	{
		std::cout << "Error creating glfw window" << std::endl;
		return -1;
	}

	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error initializing glew" << std::endl;
		return -1;
	}

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(.15f, .15f, .15f, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}