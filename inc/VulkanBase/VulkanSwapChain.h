#pragma once

#include <stdlib.h>
#include <string>
#include <assert.h>
#include <stdio.h>
#include <vector>

#include <vulkan/vulkan.h>
#include "macros.h"

/// @brief 交换链缓冲区
typedef struct SwapChainBuffer
{
	VkImage		image;
	VkImageView view;
};

/// @brief vulkan交换链
class VulkanSwapChain
{
public:
	VkFormat										colorFormat;
	VkColorSpaceKHR									colorSpace;
	VkSwapchainKHR									swapChain = VK_NULL_HANDLE;
	uint32_t										imageCount;
	std::vector<VkImage>							images;
	std::vector<SwapChainBuffer>					buffers;
	VkExtent2D										extent = {};
	uint32_t										queueNodeIndex = UINT32_MAX;
public:
	/// @brief 创建用于表示的本机平台窗口的特定于平台的表面抽象
	void InitSurface(void* platformHandle, void* platformWindow);
	
	/// @brief 设置用于交换链的实例、物理和逻辑设备，并获取所有必需的函数指针
	/// @param instance Vulkan实例
	/// @param physicalDevice 用于查询与交换链相关的属性和格式的物理设备
	/// @param device 要为其创建交换链的设备的逻辑表示形式
	void Connect(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device);
	
	/// @brief 创建swapChain并获取具有给定宽度和高度的图像
	/// @param width swapChain宽度
	/// @param height swapChain高度
	/// @param vsync 可用于强制vsync渲染     VSync屏幕撕裂
	void Create(uint32_t* width, uint32_t* height, bool vsync = false);
	
	/// @brief 获取交换链中的下一个图像
	/// @param presentCompleteSemaphore 当图像准备好使用时发出信号的信号量
	/// @param imageIndex 指向图像索引的指针，如果可以获取下一个图像，该索引将增加
	/// @note 通过将超时设置UINT64_MAX，该功能将始终等待下一个图像被获取
	/// @return 图像采集的结果
	VkResult AcquireNextImage(VkSemaphore presentCompleteSemaphore, uint32_t* imageIndex);
	
	/// @brief 将图像排队以供演示
	/// @param queue 用于显示图像的显示队列
	/// @param imageIndex 要排队显示的swapchain图像索引
	/// @param waitSemaphore 在显示图像之前等待的信号量
	VkResult QueuePresent(VkQueue queue, uint32_t imageIndex, VkSemaphore waitSemaphore = VK_NULL_HANDLE);
	
	/// @brief 销毁并释放用于交换链的Vulkan资源
	void Cleanup();
private:
	VkInstance										m_instance;
	VkDevice										m_device;
	VkPhysicalDevice								m_physicalDevice;
	VkSurfaceKHR									m_surface = VK_NULL_HANDLE;
	// 函数指针
	PFN_vkGetPhysicalDeviceSurfaceSupportKHR		fpGetPhysicalDeviceSurfaceSupportKHR;
	PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR	fpGetPhysicalDeviceSurfaceCapabilitiesKHR;
	PFN_vkGetPhysicalDeviceSurfaceFormatsKHR		fpGetPhysicalDeviceSurfaceFormatsKHR;
	PFN_vkGetPhysicalDeviceSurfacePresentModesKHR	fpGetPhysicalDeviceSurfacePresentModesKHR;
	PFN_vkCreateSwapchainKHR						fpCreateSwapchainKHR;
	PFN_vkDestroySwapchainKHR						fpDestroySwapchainKHR;
	PFN_vkGetSwapchainImagesKHR						fpGetSwapchainImagesKHR;
	PFN_vkAcquireNextImageKHR						fpAcquireNextImageKHR;
	PFN_vkQueuePresentKHR							fpQueuePresentKHR;
};