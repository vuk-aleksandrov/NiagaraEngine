#pragma once

#include <vector>

#include "VulkanCommon.h"


namespace niagara{

class FVulkanDevice;
class FVulkanSwapChain;

class FVulkanDynamicRHI
{
public:
	FVulkanDynamicRHI();
	~FVulkanDynamicRHI();

	void Init();
	void Destroy();

	void CreateInstance();
	void InitDebugger();

	void CreateSurface(void* Handle);

	void CreateDevice(VkSurfaceKHR& Surface);

	inline VkInstance& GetInstance()
	{
		return Instance;
	}

	inline FVulkanDevice& GetDevice()
	{
		return *Device;
	}

	inline FVulkanSwapChain& GetSwapChain()
	{
		return *SwapChain;
	}

	void CreateSwapChain(VkSurfaceKHR& InSurface, uint32_t WindowWidth, uint32_t WindowHeight);

private:

	VkInstance Instance;
	VkDebugUtilsMessengerEXT DebugMessenger;

	VkSurfaceKHR Surface;

	void SelectGPU(VkPhysicalDevice& GPU);

	FVulkanDevice* Device;

	FVulkanSwapChain* SwapChain;
};

}// namespace niagara
