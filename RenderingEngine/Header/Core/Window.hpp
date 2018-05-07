#pragma once

struct GLFWwindow;

class Window
{
private:

	int Width, Height;
	double XPosition, YPosition;
	double PreviousXPosition, PreviousYPosition;
	double YWheelOffset;

	float WorldTime = 0.0f;
	float DeltaTime = 0.0f;
	void UpdateDeltaTime();

	GLFWwindow* GLFWWindow;

	Window() {}

	void WindowInit();
	void WindowLoop();

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

	bool GetKeyDown(int KeyId) const;

	void SetSize(const int &WindowWidth, const int &WindowHeight);

	int GetWidth() const;
	int GetHeight() const;
	double GetMouseX() const;
	double GetMouseY() const;
	double GetYWheelOffset() const;
	float GetWorldTime() const;
	float GetDeltaTime() const;

	void ScrollCallback(double XOffset, double YOffset);

	static Window & GetSingletonWindow()
	{
		static Window SingletonWindow;
		return SingletonWindow;
	}
};