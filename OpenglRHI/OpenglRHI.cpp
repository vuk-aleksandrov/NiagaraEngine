#include "OpenglRHI.h"

#include <iostream>

#include <GLFW/glfw3.h>

FOpenglDynamicRHI::FOpenglDynamicRHI()
{
	
}

FOpenglDynamicRHI::~FOpenglDynamicRHI()
{

}

void FOpenglDynamicRHI::Init()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize OpenGL context" << std::endl;
	}
}

void FOpenglDynamicRHI::Destroy()
{

}
