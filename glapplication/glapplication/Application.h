#pragma once
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <string>

namespace GLApplication
{ 

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


class Application
{

	protected:
	bool glfwIsInitialized = false;
	GLFWwindow* glfwWindow{nullptr};
	int windowWidth;
	int windowHeight;
	std::string windowTitle;

	void SetWindowSize(int width, int height);
	void SetWindowTitle(std::string title);
	void GetFramebufferSize(int& width, int& height);
	void EnableDebug();

	virtual void OnKey(int key, int scancode, int action, int mods);
	virtual bool BindOpenGL();
	virtual bool InitializeApplication();
	virtual bool InitializeContext();
	virtual void Update();

	public:
	Application();
	Application(int width, int height, std::string title);
	virtual ~Application();

	bool Initialize();
	void Run();
	
	friend void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};

}

