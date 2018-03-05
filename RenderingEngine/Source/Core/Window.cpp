#include "Core/Window.hpp"
#include <iostream>

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Window::GetWindow().yWheelOffset += yoffset;
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
	width = 800;
	height = 600;
	window = glfwCreateWindow( width, height, "GameEngine", NULL, NULL);
	if( window == NULL )
	{
	    fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
	    glfwTerminate();
	    return;
	}



	// Get the resolution of the primary monitor
	const GLFWvidmode * vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	// Center our window
	glfwSetWindowPos(
		window,
		(vidmode->width - width) / 2,
		(vidmode->height - height) / 2
	);

	glfwMakeContextCurrent(window); // Initialize GLEW

	glfwSwapInterval(1);

	// Make the window visible
	glfwShowWindow(window);

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
	glViewport(0, 0, width, height);


	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwGetCursorPos(window, &xpos, &ypos);
	prevxpos = xpos;
	prevypos = ypos;

	glfwSetScrollCallback(window, scroll_callback);
	
	
}

void Window::WindowLoop(){

	do
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		Loop(); //Implemented by main.cpp

		// Swap buffers
		glfwSwapBuffers(window);

		prevxpos = xpos;
		prevypos = ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 );
}

void Window::Run()
{
	WindowInit();
	Init();
	WindowLoop();
	Clear();
	glfwDestroyWindow(window);
	glfwTerminate();
}


bool Window::GetKeyDown(int key)
{
	int state = glfwGetKey(window, key);
	return state == GLFW_PRESS;
}


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