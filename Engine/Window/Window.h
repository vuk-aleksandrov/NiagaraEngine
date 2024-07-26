#pragma once

//#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace niagara {
class FWindow
{
public:
	FWindow(int InWidth, int InHeight, const char* InTitle);
	~FWindow();

	void SwapBuffers();

	bool ShouldClose();
	void PollEvents();

	inline int& GetWidth()
	{
		return Width;
	}
	inline int& GetHeight()
	{
		return Height;
	}

	inline GLFWwindow* GetHandle()
	{
		return Handle;
	}

private:

	GLFWwindow* Handle;

	int Width;
	int Height;
	const char* Title;
};
}// namespace niagara