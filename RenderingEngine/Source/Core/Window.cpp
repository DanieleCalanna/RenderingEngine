#include "Core/Window.hpp"
#include <iostream>

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Window::GetSingletonWindow().SetYWheelOffset(Window::GetSingletonWindow().GetYWheelOffset()+yoffset);
}

/*
*  Initialize GLFW. Most GLFW functions will not work before doing this.
*/
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

	// Open a window and create its OpenGL context
	Width = 800;
	Height = 600;
	GLFWWindow = glfwCreateWindow( Width, Height, "Rendering Engine", NULL, NULL);
	if( GLFWWindow == NULL )
	{
	    fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
	    glfwTerminate();
	    return;
	}



	// Get the resolution of the primary monitor
	const GLFWvidmode * vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	// Center our window
	glfwSetWindowPos(
		GLFWWindow,
		0/*(vidmode->width - Width) / 2*/,
		(vidmode->height - Height) / 2
		
	);

	glfwMakeContextCurrent(GLFWWindow); // Initialize GLEW

	glfwSwapInterval(1);

	// Make the window visible
	glfwShowWindow(GLFWWindow);

	glewExperimental=true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return;
	}
	std::cout<<glGetString(GL_VERSION)<<std::endl;

	glClearColor(34.0f/255.0f, 44.0f/255.0f, 55.0f/255.0f, 1.0f); // Set background color to black and opaque
	glClearDepth(1.0f);                   // Set background depth to farthest
	glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
	glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
	glShadeModel(GL_SMOOTH);   // Enable smooth shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
	glViewport(0, 0, Width, Height);


	glfwSetInputMode(GLFWWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwGetCursorPos(GLFWWindow, &XPosition, &YPosition);
	PreviousXPosition = XPosition;
	PreviousYPosition = YPosition;

	glfwSetScrollCallback(GLFWWindow, scroll_callback);

	if (InitFunction) { InitFunction(); }
}

void Window::WindowLoop(){

	do
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		if (LoopFunction) { LoopFunction(); } //Implemented by main.cpp

		// Swap buffers
		glfwSwapBuffers(GLFWWindow);

		PreviousXPosition = XPosition;
		PreviousYPosition = YPosition;
		glfwGetCursorPos(GLFWWindow, &XPosition, &YPosition);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(GLFWWindow, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(GLFWWindow) == 0 );
}

void Window::Run()
{
	WindowInit();
	WindowLoop();
	if (ClearFunction) { ClearFunction(); }
	glfwDestroyWindow(GLFWWindow);
	glfwTerminate();
}

bool Window::GetKeyDown(int KeyId)
{
	int State = glfwGetKey(GLFWWindow, KeyId);
	return State == GLFW_PRESS;
}

void Window::SetSize(const int & WindowWidth, const int & WindowHeight)
{
	Width = WindowWidth;
	Height = WindowHeight;
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

int Window::GetWidth() { return Width; }
int Window::GetHeight() { return Height; }
double Window::GetMouseX() { return XPosition - PreviousXPosition; }
double Window::GetMouseY() { return YPosition - PreviousYPosition; }

void Window::SetYWheelOffset(double NewYWheelOffset) { YWheelOffset = NewYWheelOffset; }

double Window::GetYWheelOffset() { return YWheelOffset; }


/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
/*
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
	// Compute aspect ratio of the new window
	if (height == 0) height = 1;                // To prevent divide by 0
	GLfloat aspect = (GLfloat)width / (GLfloat)height;

	glViewport(0, 0, width, height);

	Window::GetWindow().SetSize(width, height);
}*/