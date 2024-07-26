#pragma once

#include <iostream>

#include "VulkanRHI.h"

namespace niagara::VulkanDebug {
	const std::vector<const char*> ValidationLayers =
	{ "VK_LAYER_KHRONOS_validation" };

	VkResult CreateDebugMessenger(VkInstance Instance, const VkDebugUtilsMessengerCreateInfoEXT* PtrCreateInfo,
		const VkAllocationCallbacks* PtrAllocator, VkDebugUtilsMessengerEXT* PtrDebugMessenger)
	{
		auto Func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(Instance, "vkCreateDebugUtilsMessengerEXT");
		if (Func != nullptr)
		{
			return Func(Instance, PtrCreateInfo, PtrAllocator, PtrDebugMessenger);
		}
		else
		{
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void DestroyDebugMessenger(VkInstance Instance, VkDebugUtilsMessengerEXT DebugMessenger, const VkAllocationCallbacks* PtrAllocator)
	{
		auto Func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(Instance, "vkDestroyDebugUtilsMessengerEXT");
		if (Func != nullptr)
		{
			Func(Instance, DebugMessenger, PtrAllocator);
		}
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL UserDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT MessageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT MessageType, const VkDebugUtilsMessengerCallbackDataEXT* PtrCallbackData, void* PtrUserData)
	{

		std::cerr << "Validation Layer: " << PtrCallbackData->pMessage << std::endl;

		return VK_FALSE;
	}
};


namespace niagara {
void FVulkanDynamicRHI::InitDebugger()
{
#ifdef VULKAN_DEBUG
	VkDebugUtilsMessengerCreateInfoEXT DebugMessengerInfo{};
	DebugMessengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	DebugMessengerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

	DebugMessengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

	DebugMessengerInfo.pfnUserCallback = VulkanDebug::UserDebugCallback;


	uint32_t LayerCount;
	vkEnumerateInstanceLayerProperties(&LayerCount, nullptr);

	std::vector<VkLayerProperties> AvailableLayers(LayerCount);
	vkEnumerateInstanceLayerProperties(&LayerCount, AvailableLayers.data());

	for (const char* LayerName : VulkanDebug::ValidationLayers) {
		bool LayerFound = false;

		for (const auto& layerProperties : AvailableLayers) {
			if (strcmp(LayerName, layerProperties.layerName) == 0) {
				LayerFound = true;
				break;
			}	
		}

		VK_ASSERT(!LayerFound, "No layer found!");
	}

	VK_ASSERT(VulkanDebug::CreateDebugMessenger(Instance, &DebugMessengerInfo, nullptr, &DebugMessenger), "Couldn't Create Debug Messenger!");
#endif
}

}// namespace niagara

