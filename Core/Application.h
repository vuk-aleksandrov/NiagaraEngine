#pragma once

#include <memory>

#include "OpenglRHI.h"

namespace niagara {

class FWindow;
class FRenderer;

class FApplication
{
public:
	FApplication();
	~FApplication();

	void Run();

	static std::unique_ptr<FApplication> Construct();
private:
	FWindow* Window;

	FRenderer* Renderer;

	FOpenglDynamicRHI OpenglRHI;
};
}// namespace niagara