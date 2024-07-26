#pragma once

#include "VulkanCommon.h"

#include <optional>


namespace niagara {

class FWindow;

struct FQueueFamilyIndices
{
	std::optional<uint32_t> GraphicsFamily;
	std::optional<uint32_t>  PresentFamily;

	bool IsComplete()
	{
		return GraphicsFamily.has_value() && PresentFamily.has_value();
	}
};

struct FSwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR		Capabilities;
	std::vector<VkSurfaceFormatKHR>		 Formats;
	std::vector<VkPresentModeKHR>	PresentModes;
};

class FVulkanDevice
{
public:
	FVulkanDevice(VkPhysicalDevice& GPU, VkSurfaceKHR& InSurface);
	~FVulkanDevice();

	void CreateDevice();

	void Destroy();

	FSwapChainSupportDetails SwapChainSupport(VkSurfaceKHR& Surface);

	VkSurfaceFormatKHR SelectSwapChainSurfaceFormat();
	VkPresentModeKHR SelectSwapChainPresentMode();
	VkExtent2D SelectSwapChainExtent(uint32_t WindowWidth, uint32_t WindowHeight);

	inline const FQueueFamilyIndices& GetQueueFamilies() const
	{
		return QueueFamilyIndices;
	}

	inline VkDevice& GetLogicalDevice()
	{
		return LogicalDevice;
	}

private:

	VkDevice LogicalDevice;

	VkPhysicalDevice GPU;
	FSwapChainSupportDetails GPUSwapChainSupport;

	const std::vector<const char*> DeviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	VkSurfaceKHR Surface;

	FQueueFamilyIndices QueueFamilyIndices;

	VkQueue GraphicsQueue;
	VkQueue PresentQueue;
};
}// namespace niagara

