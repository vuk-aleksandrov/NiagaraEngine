#pragma once

#include <vector>

#include "VulkanCommon.h"

namespace niagara::VulkanDebug {
	const std::vector<const char*> ValidationLayers =
	{ "VK_LAYER_KHRONOS_validation" };

	VkResult CreateDebugMessenger(
		VkInstance Instance,
		const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugUtilsMessengerEXT* pDebugMessenger
	);

	void DestroyDebugMessenger(VkInstance Instance,
		VkDebugUtilsMessengerEXT debugMessenger,
		const VkAllocationCallbacks* pAllocator
	);

	VKAPI_ATTR VkBool32 VKAPI_CALL UserDebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData
	);
}

