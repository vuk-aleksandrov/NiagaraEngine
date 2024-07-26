#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <iostream>

#include "VulkanRHI.h"
#include "VulkanCommon.h"
#include "VulkanDebug.h"
#include "VulkanDevice.h"
#include "VulkanSwapChain.h"

namespace niagara{
FVulkanDynamicRHI::FVulkanDynamicRHI()
{
}

FVulkanDynamicRHI::~FVulkanDynamicRHI()
{
}

void FVulkanDynamicRHI::Init()
{
	CreateInstance();
	InitDebugger();

	printf("Vulkan Constructed\n");
}

void FVulkanDynamicRHI::Destroy()
{
	SwapChain->Destroy();

	Device->Destroy();
	delete Device;

	VulkanDebug::DestroyDebugMessenger(Instance, DebugMessenger, nullptr);

	vkDestroySurfaceKHR(Instance, Surface, nullptr);
	vkDestroyInstance(Instance, nullptr);

	printf("Vulkan Destructed\n");
}

void FVulkanDynamicRHI::CreateInstance()
{
	VkApplicationInfo AppInfo{};
	AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	AppInfo.pApplicationName = "Niagara";
	AppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	AppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	AppInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo InstInfo{};
	InstInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	InstInfo.pApplicationInfo = &AppInfo;

	uint32_t InstanceExtensionsCount;
	const char** PtrInstanceExtensions = glfwGetRequiredInstanceExtensions(&InstanceExtensionsCount);
	std::vector<const char*> InstanceExtensions(PtrInstanceExtensions, PtrInstanceExtensions + InstanceExtensionsCount);

#ifdef VULKAN_DEBUG
	InstanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

	InstInfo.enabledExtensionCount = (uint32_t)InstanceExtensions.size();
	InstInfo.ppEnabledExtensionNames = InstanceExtensions.data();

#ifdef VULKAN_DEBUG
	InstInfo.enabledLayerCount = (uint32_t)VulkanDebug::ValidationLayers.size();
	InstInfo.ppEnabledLayerNames = VulkanDebug::ValidationLayers.data();
#endif

	VK_ASSERT(vkCreateInstance(&InstInfo, nullptr, &Instance), "Couldn't Create Instance!");
}

void FVulkanDynamicRHI::CreateSwapChain(VkSurfaceKHR& InSurface, uint32_t InWindowWidth, uint32_t InWindowHeight)
{
	SwapChain = new FVulkanSwapChain(Device, InSurface, InWindowWidth, InWindowHeight);
}

void FVulkanDynamicRHI::SelectGPU(VkPhysicalDevice& InGPU)
{
	uint32_t GPUCount;
	vkEnumeratePhysicalDevices(Instance, &GPUCount, nullptr);

	std::vector<VkPhysicalDevice> physicalDevices(GPUCount);
	vkEnumeratePhysicalDevices(Instance, &GPUCount, physicalDevices.data());

	InGPU = physicalDevices[0];
}

void FVulkanDynamicRHI::CreateSurface(void* Handle)
{
	VK_ASSERT(glfwCreateWindowSurface(Instance, (GLFWwindow*)Handle, nullptr, &Surface), "Couldn't create Surface!");
}

void FVulkanDynamicRHI::CreateDevice(VkSurfaceKHR& Surface)
{
	VkPhysicalDevice GPU;
	SelectGPU(GPU);

	Device = new FVulkanDevice(GPU, Surface);
}

}// namespace niagara
