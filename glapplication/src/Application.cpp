
#include <iostream>
#include <glapplication/Application.h>


namespace GLApplication {

Application::Application()
: Application(640, 480, "GL Application") {}

Application::Application(int width, int height, std::string title)
: windowWidth(width), windowHeight(height), windowTitle(title)
{}

Application::~Application()
{
	if (glfwWindow)
	{
		glfwDestroyWindow(glfwWindow);
	}
	if(glfwIsInitialized)
		glfwTerminate();
}


void Application::SetWindowSize(int width, int height)
{
	windowWidth = width;
	windowHeight = height;
}

void Application::SetWindowTitle(std::string title)
{
	windowTitle = title;
}

bool Application::InitializeApplication()
{
	return true;
}

bool Application::InitializeContext()
{
	return true;
}

void Application::Update()
{
}

bool Application::BindOpenGL()
{
	if (!gladLoadGL(glfwGetProcAddress))
		return false;
	return true;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	auto application = static_cast<Application*>(glfwGetWindowUserPointer(window));
	application->OnKey(key, scancode, action, mods);
}

void Application::OnKey(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(glfwWindow, GLFW_TRUE);
}


static void error_callback(int error, const char* description)
{
	std::cerr << "Error: " << description << std::endl;
}

bool Application::Initialize()
{
	glfwSetErrorCallback(error_callback);

	glfwIsInitialized = glfwInit();

	if(!glfwIsInitialized)
		return false;

	if(!InitializeApplication())
		return false;

	glfwWindow = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), NULL, NULL);
	if (!glfwWindow)
		return false;

	glfwSetWindowUserPointer(glfwWindow, this);

	glfwSetKeyCallback(glfwWindow, key_callback);

	glfwMakeContextCurrent(glfwWindow);

	if(!BindOpenGL())
		return false;

	if (!InitializeContext())
		return false;

	return true;
}

void Application::GetFramebufferSize(int& width, int& height)
{
	width = 0;
	height = 0;
	if(glfwWindow)
		glfwGetFramebufferSize(glfwWindow, &width, &height);
}

void Application::Run()
{
	while (!glfwWindowShouldClose(glfwWindow))
	{
		Update();

		glfwSwapBuffers(glfwWindow);
		glfwPollEvents();
	}
	if (glfwWindow)
	{
		glfwDestroyWindow(glfwWindow);
		glfwWindow = nullptr;
	}
}

}