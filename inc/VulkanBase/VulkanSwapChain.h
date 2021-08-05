#pragma once

#include <stdlib.h>
#include <string>
#include <assert.h>
#include <stdio.h>
#include <vector>

#include <vulkan/vulkan.h>
#include "macros.h"

/// @brief ������������
typedef struct SwapChainBuffer
{
	VkImage		image;
	VkImageView view;
};

/// @brief vulkan������
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
	/// @brief �������ڱ�ʾ�ı���ƽ̨���ڵ��ض���ƽ̨�ı������
	void InitSurface(void* platformHandle, void* platformWindow);
	
	/// @brief �������ڽ�������ʵ����������߼��豸������ȡ���б���ĺ���ָ��
	/// @param instance Vulkanʵ��
	/// @param physicalDevice ���ڲ�ѯ�뽻������ص����Ժ͸�ʽ�������豸
	/// @param device ҪΪ�䴴�����������豸���߼���ʾ��ʽ
	void Connect(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device);
	
	/// @brief ����swapChain����ȡ���и�����Ⱥ͸߶ȵ�ͼ��
	/// @param width swapChain���
	/// @param height swapChain�߶�
	/// @param vsync ������ǿ��vsync��Ⱦ     VSync��Ļ˺��
	void Create(uint32_t* width, uint32_t* height, bool vsync = false);
	
	/// @brief ��ȡ�������е���һ��ͼ��
	/// @param presentCompleteSemaphore ��ͼ��׼����ʹ��ʱ�����źŵ��ź���
	/// @param imageIndex ָ��ͼ��������ָ�룬������Ի�ȡ��һ��ͼ�񣬸�����������
	/// @note ͨ������ʱ����UINT64_MAX���ù��ܽ�ʼ�յȴ���һ��ͼ�񱻻�ȡ
	/// @return ͼ��ɼ��Ľ��
	VkResult AcquireNextImage(VkSemaphore presentCompleteSemaphore, uint32_t* imageIndex);
	
	/// @brief ��ͼ���Ŷ��Թ���ʾ
	/// @param queue ������ʾͼ�����ʾ����
	/// @param imageIndex Ҫ�Ŷ���ʾ��swapchainͼ������
	/// @param waitSemaphore ����ʾͼ��֮ǰ�ȴ����ź���
	VkResult QueuePresent(VkQueue queue, uint32_t imageIndex, VkSemaphore waitSemaphore = VK_NULL_HANDLE);
	
	/// @brief ���ٲ��ͷ����ڽ�������Vulkan��Դ
	void Cleanup();
private:
	VkInstance										m_instance;
	VkDevice										m_device;
	VkPhysicalDevice								m_physicalDevice;
	VkSurfaceKHR									m_surface = VK_NULL_HANDLE;
	// ����ָ��
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