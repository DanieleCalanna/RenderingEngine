#include "Core/Window.hpp"

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "AntTweakBar.h"

#include <iostream>


void Window::UpdateDeltaTime()
{
	float CurrentTime = (float)glfwGetTime();
	DeltaTime = CurrentTime - WorldTime;
	WorldTime = CurrentTime;
}

void Window::WindowInit()
{
	if( !glfwInit() )
	{
	    fprintf( stderr, "Failed to initialize GLFW\n" );
	    return;
	}
	// Configure our window
	glfwDefaultWindowHints(); // optional, the current window hints are already the default
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); // the window will stay hidden after creation
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); // the window will be resizable

	glfwWindowHint(GLFW_STENCIL_BITS, 1024); //antialiasing
	glfwWindowHint(GLFW_SAMPLES, 1024); //antialiasing

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing 	
	glEnable(GL_MULTISAMPLE);

	// Open a window and create its OpenGL context

	GLFWmonitor* Monitor = glfwGetPrimaryMonitor();

	const GLFWvidmode* VideoMode = glfwGetVideoMode(Monitor);
	glfwWindowHint(GLFW_RED_BITS, VideoMode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, VideoMode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, VideoMode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, VideoMode->refreshRate);

	switch (ScreenMode)
	{
	case FullScreen:
		Width = VideoMode->width;
		Height = VideoMode->height;
		GLFWWindow = glfwCreateWindow(VideoMode->width, VideoMode->height, "Rendering Engine", Monitor, NULL);
		break;
	case Windowed:
		GLFWWindow = glfwCreateWindow(Width, Height, "Rendering Engine", NULL, NULL);
		break;
	}
	if( GLFWWindow == NULL )
	{
	    std::cerr<<"Failed to open GLFW window.\n"<<std::endl;
	    glfwTerminate();
	    return;
	}
	/*
	glfwSetWindowPos(
		GLFWWindow,
		(VideoMode->width - Width) / 2,
		(VideoMode->height - Height) / 2
	);
	*/
	glfwSetWindowPos(
		GLFWWindow,
		10, 40
	);

	glfwMakeContextCurrent(GLFWWindow); // Initialize GLEW

	glfwSwapInterval(1); // v-sync on

	// Make the window visible
	glfwShowWindow(GLFWWindow);
	glfwSetWindowUserPointer(GLFWWindow, this);


	//glfwEnable(GLFW_MOUSE_CURSOR);
	//glfwEnable(GLFW_KEY_REPEAT);
	TwInit(TW_OPENGL, NULL);
	TwWindowSize(Width, Height);

	auto MouseButtonCallbackLambda = [](GLFWwindow* GLFWindow, int GLFWButton, int GLFWAction, int)
	{
		TwEventMouseButtonGLFW(GLFWButton, GLFWAction);
	};
	glfwSetMouseButtonCallback(GLFWWindow, (GLFWmousebuttonfun)MouseButtonCallbackLambda);

	auto ScrollCallbackLambda = [](GLFWwindow* GLFWindow, double XOffset, double YOffset)
	{
		Window* WindowUserPointer = static_cast<Window*>(glfwGetWindowUserPointer(GLFWindow));
		WindowUserPointer->ScrollCallback(XOffset, YOffset);
		TwEventMouseWheelGLFW((int)WindowUserPointer->YWheelOffset);
	};
	glfwSetScrollCallback(GLFWWindow, ScrollCallbackLambda);
	
	auto KeyCallbackLambda = [](GLFWwindow* GLFWindow, int GLFWKey, int, int GLFWAction, int)
	{
		TwEventKeyGLFW(GLFWKey, GLFWAction);
	};
	glfwSetKeyCallback(GLFWWindow, (GLFWkeyfun)KeyCallbackLambda);

	auto CharCallbackLambda = [](GLFWwindow* GLFWindow, unsigned int GLFWChar)
	{
		TwEventCharGLFW(GLFWChar, GLFW_PRESS);
	};
	glfwSetCharCallback(GLFWWindow, (GLFWcharfun)CharCallbackLambda);

	auto WindowSizeCallbackLambda = [](GLFWwindow* GLFWindow, int Width, int Height)
	{
		Window* WindowUserPointer = static_cast<Window*>(glfwGetWindowUserPointer(GLFWindow));
		WindowUserPointer->SetSize(Width, Height);
	};
	glfwSetWindowSizeCallback(GLFWWindow, (GLFWwindowsizefun)WindowSizeCallbackLambda);

	glewExperimental=true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return;
	}
	std::cout<<glGetString(GL_VERSION)<<std::endl;
	
	//glClearColor(33.0f/255.0f, 43.0f/255.0f, 53.0f/255.0f, 1.0f); // Set background color to black and opaque
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearDepth(1.0f);                   // Set background depth to farthest
	glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
	glShadeModel(GL_SMOOTH);   // Enable smooth shading
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
	glViewport(0, 0, Width, Height);
	
	glfwSetInputMode(GLFWWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glfwGetCursorPos(GLFWWindow, &XPosition, &YPosition);
	PreviousXPosition = XPosition;
	PreviousYPosition = YPosition;

	if (InitFunction) { InitFunction(); }
}

void Window::WindowLoop(){

	do
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		PreviousXPosition = XPosition;
		PreviousYPosition = YPosition;

		glfwGetCursorPos(GLFWWindow, &XPosition, &YPosition);

		TwEventMousePosGLFW((int)XPosition, (int)YPosition);

		if (LoopFunction) { LoopFunction(); } //Implemented by main.cpp
		TwDraw();

		// Swap buffers
		glfwSwapBuffers(GLFWWindow);

		UpdateDeltaTime();

		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(GLFWWindow, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(GLFWWindow) == 0 );
}

void Window::ScrollCallback(double XOffset, double YOffset)
{
	YWheelOffset += YOffset;
}

void Window::SetMouseEnabled(bool Enabled)
{
	MouseEnabled = Enabled;
	if (MouseEnabled)
	{
		glfwSetInputMode(GLFWWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else
	{
		glfwSetInputMode(GLFWWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}

void Window::Run()
{
	WindowInit();
	WindowLoop();
	if (ClearFunction) { ClearFunction(); }
	TwTerminate();
	glfwDestroyWindow(GLFWWindow);
	glfwTerminate();
}

bool Window::GetKeyDown(int KeyId) const
{
	int State = glfwGetKey(GLFWWindow, KeyId);
	return State == GLFW_PRESS;
}

void Window::SetSize(const int & WindowWidth, const int & WindowHeight)
{
	Width = WindowWidth;
	Height = WindowHeight;
	TwWindowSize(Width, Height);
	glViewport(0, 0, Width, Height);
}

void Window::SetInitFunction(void(*InitFunctionToCall) (void))
{
	InitFunction = InitFunctionToCall;
}
void Window::SetLoopFunction(void(*LoopFunctionToCall) (void))
{
	LoopFunction = LoopFunctionToCall;
}
void Window::SetClearFunction(void(*ClearFunctionToCall) (void))
{
	ClearFunction = ClearFunctionToCall;
}

int Window::GetWidth() const { return Width; }
int Window::GetHeight() const { return Height; }
double Window::GetMouseX() const { return XPosition - PreviousXPosition; }
double Window::GetMouseY() const { return YPosition - PreviousYPosition; }

double Window::GetYWheelOffset() const { return YWheelOffset; }

float Window::GetWorldTime() const { return WorldTime; }

float Window::GetDeltaTime() const { return DeltaTime; }

void Window::SetScreenMode(const EScreenMode & ScreenMode)
{
	this->ScreenMode = ScreenMode;
}