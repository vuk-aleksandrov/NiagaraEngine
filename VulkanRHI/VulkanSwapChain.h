#pragma once

#include <vector>

#include "VulkanCommon.h"

namespace niagara {

class FVulkanDevice;

class FVulkanSwapChain
{
public:
	FVulkanSwapChain(FVulkanDevice* InDevice, VkSurfaceKHR& InSurface,uint32_t WindowWidth, uint32_t WindowHeight);
	~FVulkanSwapChain();

	void RecreateSwapChain();
	void Destroy();


private:
	VkSwapchainKHR SwapChain;

	FVulkanDevice* Device;
	VkSurfaceKHR Surface;

	std::vector<VkImage> SwapChainImages;
	VkFormat SwapChainImageFormat;
	VkExtent2D SwapChainExtent;
};
}// namespace niagara

