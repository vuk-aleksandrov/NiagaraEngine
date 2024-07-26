#pragma once

#include "VulkanSwapChain.h"
#include "VulkanRHI.h"
#include "VulkanCommon.h"
#include "VulkanDevice.h"
#include <Window.h>

namespace niagara {
FVulkanSwapChain::FVulkanSwapChain(FVulkanDevice* InDevice, VkSurfaceKHR& InSurface, uint32_t WindowWidth, uint32_t WindowHeight)
	: Device(InDevice),
	  Surface(InSurface)
{
	auto SwapChainSupport = Device->SwapChainSupport(Surface);

	VkSurfaceFormatKHR SurfaceFormat = Device->SelectSwapChainSurfaceFormat();
	VkPresentModeKHR PresentMode = Device->SelectSwapChainPresentMode();
	VkExtent2D Extent = Device->SelectSwapChainExtent(WindowWidth, WindowHeight);

	uint32_t ImageCount = SwapChainSupport.Capabilities.minImageCount + 1;
	if (SwapChainSupport.Capabilities.maxImageCount > 0 && ImageCount > SwapChainSupport.Capabilities.maxImageCount) {
		ImageCount = SwapChainSupport.Capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR SwapChainInfo{};
	SwapChainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	SwapChainInfo.surface = Surface;

	SwapChainInfo.minImageCount = ImageCount;
	SwapChainInfo.imageFormat = SurfaceFormat.format;
	SwapChainInfo.imageColorSpace = SurfaceFormat.colorSpace;
	SwapChainInfo.imageExtent = Extent;
	SwapChainInfo.imageArrayLayers = 1;
	SwapChainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	FQueueFamilyIndices indices = Device->GetQueueFamilies();
	uint32_t QueueFamilyIndices[2] = { indices.GraphicsFamily.value(), indices.PresentFamily.value() };

	if (indices.GraphicsFamily != indices.PresentFamily) {
		SwapChainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		SwapChainInfo.queueFamilyIndexCount = 2;
		SwapChainInfo.pQueueFamilyIndices = QueueFamilyIndices;
	}
	else {
		SwapChainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	SwapChainInfo.preTransform = SwapChainSupport.Capabilities.currentTransform;
	SwapChainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	SwapChainInfo.presentMode = PresentMode;
	SwapChainInfo.clipped = VK_TRUE;

	SwapChainInfo.oldSwapchain = VK_NULL_HANDLE;

	VK_ASSERT(vkCreateSwapchainKHR(Device->GetLogicalDevice(), &SwapChainInfo, nullptr, &SwapChain), "Failed to create Swap Chain!");

	vkGetSwapchainImagesKHR(Device->GetLogicalDevice(), SwapChain, &ImageCount, nullptr);
	SwapChainImages.resize(ImageCount);
	vkGetSwapchainImagesKHR(Device->GetLogicalDevice(), SwapChain, &ImageCount, SwapChainImages.data());

	SwapChainImageFormat = SurfaceFormat.format;
	SwapChainExtent = Extent;

	printf("Swap Chain created!\n");
}

FVulkanSwapChain::~FVulkanSwapChain()
{
	Destroy();
	printf("Swap Chain Destroyed!\n");
}

void FVulkanSwapChain::RecreateSwapChain()
{

}

void FVulkanSwapChain::Destroy()
{
	vkDestroySwapchainKHR(Device->GetLogicalDevice(), SwapChain, nullptr);
}
}// namespace niagara