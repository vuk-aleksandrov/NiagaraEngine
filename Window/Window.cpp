#include "Window.h"

namespace niagara {
FWindow::FWindow(int InWidth, int InHeight, const char* InTitle)
	: Width(InWidth)
	, Height(InHeight)
	, Title(InTitle)
{

	glfwInit();

	//glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	Handle = glfwCreateWindow(Width, Height, Title, nullptr, nullptr);
	glfwMakeContextCurrent(Handle);
}

FWindow::~FWindow()
{
	glfwDestroyWindow(Handle);

	glfwTerminate();
}

void FWindow::SwapBuffers()
{
	glfwSwapBuffers(Handle);
}

bool FWindow::ShouldClose()
{
	return glfwWindowShouldClose(Handle);
}

void FWindow::PollEvents()
{
	glfwPollEvents();
}
}// namespace niagara