#pragma once

#include <exception>
#include <assert.h>
#include <algorithm>
#include <cstring>
#include <vector>
#include "vulkan/vulkan.h"
#include "macros.h"

/// @brief 队列族索引
struct QueueFamilyIndices
{
	uint32_t graphics;
	uint32_t compute;
};


class VulkanDevice
{
public:
	VkPhysicalDevice						physicalDevice;					// 物理设备
	VkDevice								logicalDevice;					// 逻辑设备
	VkPhysicalDeviceProperties				properties;						// 物理设备属性
	VkPhysicalDeviceFeatures				features;						// 物理设备特性
	VkPhysicalDeviceFeatures				enabledFeatures;				// 启用的特性
	VkPhysicalDeviceMemoryProperties		memoryProperties;				// 内存属性
	std::vector<VkQueueFamilyProperties>	queueFamilyProperties;			// 队列族属性
	VkCommandPool							commandPool = VK_NULL_HANDLE;	// 命令池
	QueueFamilyIndices						queueFamilyIndices;				// 队列族索引

public:
	/// @brief vulkan设备
	/// @param [in ] physicalDevice 要使用的物理设备
	VulkanDevice(VkPhysicalDevice physicalDevice);
	~VulkanDevice();

public:
	/// @brief 获取已设置所有请求的属性位的内存类型的索引
	/// @param [in ] typeBits 设置请求的资源支持的每种内存类型
	/// @param [in ] properties 要请求的内存类型的属性
	/// @param [in ] memTypeFound 如果找到匹配的内存类型，则指向设置为 true 的 bool 的指针
	/// @return 请求的内存类型的索引
	uint32_t GetMemoryType(uint32_t typeBits, VkMemoryPropertyFlags properties, VkBool32* memTypeFound = nullptr);

	/// @brief 获取支持请求的队列标志的队列族的索引
	/// @param [in ] queueFlags [in ] 用于查找队列族索引的队列标志
	/// @return 与标志匹配的队列族索引的索引
	uint32_t GetQueueFamilyIndex(VkQueueFlagBits queueFlags);

	/// @brief 基于物理设备创建逻辑设备，并获取默认队列系列索引
	/// @param [in ] enabledFeatures 可用于在设备创建时启用某些功能
	/// @param [in ] requestedQueueTypes 指定要从设备请求的队列类型的位标志
	/// @return 设备创建调用的VkResult
	VkResult CreateLogicalDevice(VkPhysicalDeviceFeatures enabledFeatures, std::vector<const char*> enabledExtensions, VkQueueFlags requestedQueueTypes = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);
	
	/// @brief 在设备上创建缓冲区
	/// @param [in ] usageFlags 缓冲区的使用标志位掩码（即索引、顶点、统一缓冲区)
	/// @param [in ] memoryPropertyFlags 此缓冲区的内存属性（即设备本地、主机可见、一致）
	/// @param [in ] size 缓冲区的大小
	/// @param [in ] buffer 指向函数获取的缓冲区句柄的指针
	/// @param [in ] memory 指向函数获取的内存句柄的指针
	/// @param [in ] data 指向创建后应复制到缓冲区的数据的指针(如果未指定,不复制数据）
	/// @return 如果已创建缓冲区句柄和内存并且已复制(可选的)数据返回成功(VK_SUCCESS)
	VkResult CreateBuffer(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size, VkBuffer* buffer, VkDeviceMemory* memory, void* data = nullptr);
	
	/// @brief 为命令缓冲区创建命令池
	/// @param [in ] queueFamilyIndex 命令池的队列族索引
	/// @param [in ] createFlags 命令池创建标志(可不创建,默认VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT)
	/// @note 从创建的池中分配的命令缓冲区只能提交到具有相同族索引的队列
	/// @return A handle to the created command buffer
	VkCommandPool CreateCommandPool(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags createFlags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
	
	/// @brief 从命令池中分配命令缓冲区
	/// @param [in ] level 新命令缓冲区的等级(主或辅助)
	/// @param [in ] begin 如果为true，将在新的命令缓冲区(vkBeginCommandBuffer)上创建(默认false)
	VkCommandBuffer CreateCommandBuffer(VkCommandBufferLevel level, bool begin = false);
	
	/// @brief 创建命令缓冲区
	/// @param [in ] commandBuffer 从commandBuffer开始创建
	void BeginCommandBuffer(VkCommandBuffer commandBuffer);

	/// @brief 完成命令缓冲区记录并将其提交到队列
	/// @param [in ] commandBuffer 要刷新的命令缓冲区
	/// @param [in ] queue 将命令缓冲区提交到的队列
	/// @param [in ] free 提交命令缓冲区后释放它(默认true)
	/// @note 命令缓冲区提交到的队列必须与从中分配的池来自同一个族索引
	void FlushCommandBuffer(VkCommandBuffer commandBuffer, VkQueue queue, bool free = true);
};