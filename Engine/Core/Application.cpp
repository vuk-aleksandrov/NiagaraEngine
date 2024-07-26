#pragma once

#include "Application.h"


#include "Window.h"
#include "Renderer.h"

namespace niagara {
FApplication::FApplication()
{
	Window = new FWindow(800 ,600,"Niagara");

	Renderer = new FRenderer();

	OpenglRHI.Init();

}

FApplication::~FApplication()
{
	OpenglRHI.Destroy();

	delete Renderer;
	delete Window;
}

void FApplication::Run()
{
	while (!Window->ShouldClose())
	{
		Window->SwapBuffers();

		Window->PollEvents();
	}
}

std::unique_ptr<FApplication> FApplication::Construct()
{
	return std::make_unique<FApplication>();
}
}// namespace niagara