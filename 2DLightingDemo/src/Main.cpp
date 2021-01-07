#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static int w_width = 900, w_height = 900;

GLuint createShaderProgram(const std::string& vpath, const std::string& fpath);

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

    //Handle window initialization
	GLFWwindow* window = glfwCreateWindow(w_width, w_height, "2D Lighting demo!", NULL, NULL);
	if (!window)
	{
		std::cout << "Error creating glfw window" << std::endl;
		return -1;
	}

	glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error initializing glew" << std::endl;
		return -1;
	}

    //SHADERS ---------------------------------------
    GLuint sceneProgram = createShaderProgram("shaders/scene.vert", "shaders/scene.frag");
    glUseProgram(sceneProgram);
    //-----------------------------------------------

    //VAO STUFF -------------------------------------
    float verts[] = {
        -0.5f, -0.5f,   0.0f, 0.0f,
         0.5f, -0.5f,   1.0f, 0.0f,
         0.0f,  0.5f,   0.5f, 1.0f
    };

    GLuint scene_vao, scene_vbo;
    glGenVertexArrays(1, &scene_vao);
    glBindVertexArray(scene_vao);

    glGenBuffers(1, &scene_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, scene_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);

    //-----------------------------------------------

    //LOAD TEXTURE ----------------------------------
    int width, height, nrChannels;
    unsigned char* data = stbi_load("assets/stone.jpg", &width, &height, &nrChannels, 0);
    if (!data)
    {
        std::cout << "Couldn't load texture" << std::endl;
        return -1;
    }

    GLuint stone_tex;
    glGenTextures(1, &stone_tex);
    glBindTexture(GL_TEXTURE_2D, stone_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

    //-----------------------------------------------

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(.15f, .15f, .15f, 1);
		glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(sceneProgram);
        glBindVertexArray(scene_vao);
        glBindTexture(GL_TEXTURE_2D, stone_tex);
        glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}




GLuint createShaderProgram(const std::string& vspath, const std::string& fspath)
{
    std::string vscode = "", fscode = "";
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit || std::ifstream::badbit); //Set up io exceptions
    try
    {
        shaderFile.open(vspath);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf(); //Read buffer into the string stream
        shaderFile.close();
        vscode = shaderStream.str();

        shaderFile.open(fspath);
        shaderStream.str(""); //remove old data
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        fscode = shaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "Error reading shader file " << vspath << std::endl;
    }
    const char* c_vscode = vscode.c_str();
    const char* c_fscode = fscode.c_str();

    //2. attach code to shader
    GLuint vshader = glCreateShader(GL_VERTEX_SHADER), fshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vshader, 1, &c_vscode, NULL);
    glShaderSource(fshader, 1, &c_fscode, NULL);

    //3. compile and report errors.
    GLchar log[1024];
    GLint success;
    glCompileShader(vshader);
    glGetShaderiv(vshader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vshader, 1024, NULL, log);
        std::cout << vspath << ": Error compiling Vertex shader: " << log << std::endl;
        return -1;
    }
    glCompileShader(fshader);
    glGetShaderiv(fshader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fshader, 1024, NULL, log);
        std::cout << fspath << ": Error compiling Fragment shader: " << log << std::endl;
        return -1;
    }

    //4. Attach to a program
    GLuint program = glCreateProgram();
    glAttachShader(program, vshader);
    glAttachShader(program, fshader);
    glValidateProgram(program);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 1024, NULL, log);
        std::cout << "Error linking program: " << log << std::endl;
        return -1;
    }
    glDeleteShader(vshader);
    glDeleteShader(fshader);
    return program;
}