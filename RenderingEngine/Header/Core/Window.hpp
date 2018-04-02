#pragma once

struct GLFWwindow;

class Window
{
private:

	int Width, Height;
	double XPosition, YPosition;
	double PreviousXPosition, PreviousYPosition;
	double YWheelOffset;

	Window() {}

	void WindowInit();
	void WindowLoop();

	GLFWwindow* GLFWWindow;

	void(*InitFunction) (void) = nullptr;
	void(*LoopFunction) (void) = nullptr;
	void(*ClearFunction) (void) = nullptr;
public:

	Window(Window const&) = delete;
	void operator=(Window const&) = delete;

	void SetInitFunction(void(*InitFunctionToCall) (void));
	void SetLoopFunction(void(*LoopFunctionToCall) (void));
	void SetClearFunction(void(*ClearFunctionToCall) (void));
	
	void Run();

	bool GetKeyDown(int KeyId);

	void SetSize(const int &WindowWidth, const int &WindowHeight);

	int GetWidth();
	int GetHeight();
	double GetMouseX();
	double GetMouseY();
	void SetYWheelOffset(double NewYWheelOffset);
	double GetYWheelOffset();

	static Window & GetSingletonWindow()
	{
		static Window SingletonWindow;
		return SingletonWindow;
	}
};