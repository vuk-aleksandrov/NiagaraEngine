#pragma once

#include <vector>
#include <algorithm>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VulkanDevice.h"
#include "VulkanCommon.h"
#include "VulkanDebug.h"
#include "Window.h"


namespace niagara {

	FVulkanDevice::FVulkanDevice(VkPhysicalDevice& InGPU,VkSurfaceKHR& InSurface)
		: GPU(InGPU)
		, Surface(InSurface)
	{
		CreateDevice();
		printf("Device Created\n");
	}

	FVulkanDevice::~FVulkanDevice()
	{
		printf("Device Destroyed\n");
	}

void FVulkanDevice::CreateDevice()
{
	uint32_t QueueFamilyCount;
	vkGetPhysicalDeviceQueueFamilyProperties(GPU, &QueueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(QueueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(GPU, &QueueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			QueueFamilyIndices.GraphicsFamily = i;
		}

		VkBool32 PresentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(GPU, i, Surface, &PresentSupport);

		if (PresentSupport) {
			QueueFamilyIndices.PresentFamily = i;
		}

		if (QueueFamilyIndices.IsComplete()) {
			break;
		}

		i++;
	}

	VkDeviceQueueCreateInfo DeviceQueueInfo{};
	DeviceQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	DeviceQueueInfo.queueFamilyIndex = QueueFamilyIndices.GraphicsFamily.value();
	DeviceQueueInfo.queueCount = 1;

	float QueuePriority = 1.0f;
	DeviceQueueInfo.pQueuePriorities = &QueuePriority;

	VkPhysicalDeviceFeatures DeviceFeatures{};

	VkDeviceCreateInfo LogicalDeviceInfo{};
	LogicalDeviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	LogicalDeviceInfo.queueCreateInfoCount = 1;
	LogicalDeviceInfo.pQueueCreateInfos = &DeviceQueueInfo;

#ifdef VULKAN_DEBUG
	LogicalDeviceInfo.enabledLayerCount = VulkanDebug::ValidationLayers.size();
	LogicalDeviceInfo.ppEnabledLayerNames = VulkanDebug::ValidationLayers.data();
#endif

	LogicalDeviceInfo.enabledExtensionCount = DeviceExtensions.size();
	LogicalDeviceInfo.ppEnabledExtensionNames = DeviceExtensions.data();
	LogicalDeviceInfo.pEnabledFeatures = &DeviceFeatures;

	VK_ASSERT(vkCreateDevice(GPU, &LogicalDeviceInfo, nullptr, &LogicalDevice), "Couldn't Create Logical Device!");

	vkGetDeviceQueue(LogicalDevice, QueueFamilyIndices.GraphicsFamily.value(), 0, &GraphicsQueue);
	vkGetDeviceQueue(LogicalDevice, QueueFamilyIndices.PresentFamily.value(), 0, &PresentQueue);
}

void FVulkanDevice::Destroy()
{
	vkDestroyDevice(LogicalDevice, nullptr);
}

FSwapChainSupportDetails FVulkanDevice::SwapChainSupport(VkSurfaceKHR& Surface)
{
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(GPU, Surface, &GPUSwapChainSupport.Capabilities);

	uint32_t FormatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(GPU, Surface, &FormatCount, nullptr);

	if (FormatCount != 0) 
	{
		GPUSwapChainSupport.Formats.resize(FormatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(GPU, Surface, &FormatCount, GPUSwapChainSupport.Formats.data());
	}

	uint32_t PresentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(GPU, Surface, &PresentModeCount, nullptr);

	if (PresentModeCount != 0) 
	{
		GPUSwapChainSupport.PresentModes.resize(PresentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(GPU, Surface, &PresentModeCount, GPUSwapChainSupport.PresentModes.data());
	}

	return GPUSwapChainSupport;
}

VkSurfaceFormatKHR FVulkanDevice::SelectSwapChainSurfaceFormat()
{
	for (const auto& AvailableFormat : GPUSwapChainSupport.Formats) 
	{
		if (AvailableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && AvailableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return AvailableFormat;
		}
	}

	return GPUSwapChainSupport.Formats[0];
}

VkPresentModeKHR FVulkanDevice::SelectSwapChainPresentMode()
{
	for (const auto& AvailablePresentMode : GPUSwapChainSupport.PresentModes) 
	{
		if (AvailablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) 
		{
			return AvailablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D FVulkanDevice::SelectSwapChainExtent(uint32_t WindowWidth, uint32_t WindowHeight)
{
	if (GPUSwapChainSupport.Capabilities.currentExtent.width != UINT32_MAX) 
	{
		return GPUSwapChainSupport.Capabilities.currentExtent;
	}

	VkExtent2D ActualExtent = {
		WindowWidth,
		WindowHeight
	};

	ActualExtent.width = std::clamp(ActualExtent.width, GPUSwapChainSupport.Capabilities.minImageExtent.width,
									GPUSwapChainSupport.Capabilities.maxImageExtent.width);
	ActualExtent.height = std::clamp(ActualExtent.height, GPUSwapChainSupport.Capabilities.minImageExtent.height, 
									GPUSwapChainSupport.Capabilities.maxImageExtent.height);

	return ActualExtent;
}

}// namespace niagara