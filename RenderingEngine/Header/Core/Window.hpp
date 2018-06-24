#pragma once

struct GLFWwindow;

enum EScreenMode { FullScreen, Windowed };

class Window
{
private:

	EScreenMode ScreenMode = Windowed;
	int Width = 800, Height = 600;
	double XPosition = 0.0;
	double YPosition = 0.0;
	double PreviousXPosition = 0.0;
	double PreviousYPosition = 0.0;
	double YWheelOffset = 0.0;
	bool MouseEnabled = true;
	
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

	void SetScreenMode(const EScreenMode& ScreenMode);

	void ScrollCallback(double XOffset, double YOffset);

	void SetMouseEnabled(bool Enabled);

	static Window & GetSingletonWindow()
	{
		static Window SingletonWindow;
		return SingletonWindow;
	}
};