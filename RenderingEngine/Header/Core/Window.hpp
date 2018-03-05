#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"


void Init(); //To implement in main.cpp
void Loop(); //To implement in main.cpp
void Clear(); //To implement in main.cpp


class Window{
private:
	int width, height;
	double xpos, ypos;
	double prevxpos, prevypos;

	Window(){}


	void WindowInit();
	void WindowLoop();
public:
	
	Window(Window const&) = delete;
	void operator=(Window const&) = delete;

	GLFWwindow* window;
	double yWheelOffset;

	void Destroy();
	void Run();


	bool GetKeyDown(int key);

	void SetSize(const int &width, const int &height){
		this->width = width;
		this->height = height;
	}

	int Width(){ return width; }
	int Height(){ return height; }

	double getMouseX(){ return xpos-prevxpos; }
	double getMouseY(){ return ypos-prevypos; }

	static Window& GetWindow(){
		static Window window;
		return window;
	}
};