#pragma once

#include <exception>
#include <assert.h>
#include <algorithm>
#include <cstring>
#include <vector>
#include "vulkan/vulkan.h"
#include "macros.h"

/// @brief ����������
struct QueueFamilyIndices
{
	uint32_t graphics;
	uint32_t compute;
};


class VulkanDevice
{
public:
	VkPhysicalDevice						physicalDevice;					// �����豸
	VkDevice								logicalDevice;					// �߼��豸
	VkPhysicalDeviceProperties				properties;						// �����豸����
	VkPhysicalDeviceFeatures				features;						// �����豸����
	VkPhysicalDeviceFeatures				enabledFeatures;				// ���õ�����
	VkPhysicalDeviceMemoryProperties		memoryProperties;				// �ڴ�����
	std::vector<VkQueueFamilyProperties>	queueFamilyProperties;			// ����������
	VkCommandPool							commandPool = VK_NULL_HANDLE;	// �����
	QueueFamilyIndices						queueFamilyIndices;				// ����������

public:
	/// @brief vulkan�豸
	/// @param [in ] physicalDevice Ҫʹ�õ������豸
	VulkanDevice(VkPhysicalDevice physicalDevice);
	~VulkanDevice();

public:
	/// @brief ��ȡ�������������������λ���ڴ����͵�����
	/// @param [in ] typeBits �����������Դ֧�ֵ�ÿ���ڴ�����
	/// @param [in ] properties Ҫ������ڴ����͵�����
	/// @param [in ] memTypeFound ����ҵ�ƥ����ڴ����ͣ���ָ������Ϊ true �� bool ��ָ��
	/// @return ������ڴ����͵�����
	uint32_t GetMemoryType(uint32_t typeBits, VkMemoryPropertyFlags properties, VkBool32* memTypeFound = nullptr);

	/// @brief ��ȡ֧������Ķ��б�־�Ķ����������
	/// @param [in ] queueFlags [in ] ���ڲ��Ҷ����������Ķ��б�־
	/// @return ���־ƥ��Ķ���������������
	uint32_t GetQueueFamilyIndex(VkQueueFlagBits queueFlags);

	/// @brief ���������豸�����߼��豸������ȡĬ�϶���ϵ������
	/// @param [in ] enabledFeatures ���������豸����ʱ����ĳЩ����
	/// @param [in ] requestedQueueTypes ָ��Ҫ���豸����Ķ������͵�λ��־
	/// @return �豸�������õ�VkResult
	VkResult CreateLogicalDevice(VkPhysicalDeviceFeatures enabledFeatures, std::vector<const char*> enabledExtensions, VkQueueFlags requestedQueueTypes = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);
	
	/// @brief ���豸�ϴ���������
	/// @param [in ] usageFlags ��������ʹ�ñ�־λ���루�����������㡢ͳһ������)
	/// @param [in ] memoryPropertyFlags �˻��������ڴ����ԣ����豸���ء������ɼ���һ�£�
	/// @param [in ] size �������Ĵ�С
	/// @param [in ] buffer ָ������ȡ�Ļ����������ָ��
	/// @param [in ] memory ָ������ȡ���ڴ�����ָ��
	/// @param [in ] data ָ�򴴽���Ӧ���Ƶ������������ݵ�ָ��(���δָ��,���������ݣ�
	/// @return ����Ѵ���������������ڴ沢���Ѹ���(��ѡ��)���ݷ��سɹ�(VK_SUCCESS)
	VkResult CreateBuffer(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size, VkBuffer* buffer, VkDeviceMemory* memory, void* data = nullptr);
	
	/// @brief Ϊ����������������
	/// @param [in ] queueFamilyIndex ����صĶ���������
	/// @param [in ] createFlags ����ش�����־(�ɲ�����,Ĭ��VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT)
	/// @note �Ӵ����ĳ��з�����������ֻ���ύ��������ͬ�������Ķ���
	/// @return A handle to the created command buffer
	VkCommandPool CreateCommandPool(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags createFlags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
	
	/// @brief ��������з����������
	/// @param [in ] level ����������ĵȼ�(������)
	/// @param [in ] begin ���Ϊtrue�������µ��������(vkBeginCommandBuffer)�ϴ���(Ĭ��false)
	VkCommandBuffer CreateCommandBuffer(VkCommandBufferLevel level, bool begin = false);
	
	/// @brief �����������
	/// @param [in ] commandBuffer ��commandBuffer��ʼ����
	void BeginCommandBuffer(VkCommandBuffer commandBuffer);

	/// @brief ������������¼�������ύ������
	/// @param [in ] commandBuffer Ҫˢ�µ��������
	/// @param [in ] queue ����������ύ���Ķ���
	/// @param [in ] free �ύ����������ͷ���(Ĭ��true)
	/// @note ��������ύ���Ķ��б�������з���ĳ�����ͬһ��������
	void FlushCommandBuffer(VkCommandBuffer commandBuffer, VkQueue queue, bool free = true);
};