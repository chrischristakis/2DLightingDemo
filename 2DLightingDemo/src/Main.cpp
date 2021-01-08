#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    //IMGUI SETUP------------------------------------
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    //-----------------------------------------------

    //SHADERS ---------------------------------------
    GLuint sceneProgram = createShaderProgram("shaders/scene.vert", "shaders/scene.frag");
    GLuint lightProgram = createShaderProgram("shaders/light.vert", "shaders/light.frag");
    glUseProgram(lightProgram);

    std::map<std::string, GLint> locations;
    locations["lightPos"] = glGetUniformLocation(lightProgram, "lightPos");
    locations["lightColor"] = glGetUniformLocation(lightProgram, "lightColor");
    locations["light_radius"] =   glGetUniformLocation(lightProgram, "light_radius");
    //-----------------------------------------------


    // SCENE VAO STUFF -------------------------------------
    float verts[] = {
        //pos           //tc
        -1.0f, -1.0f,   0.15f, 0.0f,
         1.0f, -1.0f,   0.85f, 0.0f,
        -1.0f,  1.0f,   0.15f, 1.0f,
        -1.0f,  1.0f,   0.15f, 1.0f,
         1.0f, -1.0f,   0.85f, 0.0f,
         1.0f,  1.0f,   0.85f, 1.0f
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

    //LIGHT VAO STUFF -------------------------------
    float light_verts[] = {
        //pos       
        -1.0f, -1.0f,
         1.0f, -1.0f,
        -1.0f,  1.0f,
        -1.0f,  1.0f,
         1.0f, -1.0f,
         1.0f,  1.0f,
    };

    GLuint light_vao, light_vbo;
    glGenVertexArrays(1, &light_vao);
    glBindVertexArray(light_vao);

    glGenBuffers(1, &light_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, light_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(light_verts), light_verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);
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

    bool show_demo_window = true;
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

        //IMGUI frame init
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        
        //IMGUI FRAME-----------------------------------------
        static int light_x = 450, light_y = 450, light_radius = 100;
        ImGui::Begin("Light params");                          // Create a window called "Hello, world!" and append into it.
        ImGui::SliderInt("Pos_X", &light_x, 0, w_width);
        ImGui::SliderInt("Pos_Y", &light_y, 0, w_height);
        ImGui::SliderInt("light_radius", &light_radius, 0, 1000);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
        //----------------------------------------------------

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

        //glUseProgram(sceneProgram);
        //glBindVertexArray(scene_vao);
        //glBindTexture(GL_TEXTURE_2D, stone_tex);
        glBindVertexArray(light_vao);
        glUseProgram(lightProgram);
        glUniform4f(locations["lightColor"], 0.0, 0.0, 1.0, 1);
        glUniform2f(locations["lightPos"], light_x, light_y);
        glUniform1i(locations["light_radius"], light_radius);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glUniform4f(locations["lightColor"], 1.0, 0.0, 0.0, 1);
        glUniform2f(locations["lightPos"], 300, 300);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        //glUniform4f(locations["lightColor"], 0.0, 0.0, 1.0, 0.5);
        //glDrawArrays(GL_TRIANGLES, 0, 6);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
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