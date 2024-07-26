#pragma once

#include <vulkan/vulkan.h>
#include <iostream>

#ifndef _NDEBUG
	#define VULKAN_DEBUG
#endif

#define VK_ASSERT(cnd, msg)\
	if((cnd)!=VK_SUCCESS){\
		printf(msg);\
		__debugbreak();\
	}
