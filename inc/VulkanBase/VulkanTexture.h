#pragma once

#include "VulkanBaseDef.h"

#include <stdlib.h>
#include <string>
#include <fstream>
#include <vector>

#include "vulkan/vulkan.h"
#include "macros.h"
#include "VulkanDevice.h"

#include <gli/gli.hpp>


/// @brief vulkan纹理
class DLLEXPORTCLASS VulkanTexture
{
public:
	VulkanDevice* device;
	VkImage						image = VK_NULL_HANDLE;
	VkImageLayout				imageLayout;
	VkDeviceMemory				deviceMemory;
	VkImageView					view;
	uint32_t					width, height;
	uint32_t					mipLevels;
	uint32_t					layerCount;
	VkDescriptorImageInfo		descriptor;
	VkSampler					sampler;

	/// @brief 更新描述符
	void UpdateDescriptor();

	/// @brief 销毁
	void Destroy();
};


/// @brief vulkan2d纹理
class DLLEXPORTCLASS VulkanTexture2D : public VulkanTexture
{
public:
	/// @brief 从文件加载
	/// @param [in ] filename 文件路径
	/// @param [in ] format 格式
	/// @param [in ] device 物理设备指针
	/// @param [in ] copyQueue 拷贝队列
	/// @param [in ] imageUsageFlags 图像使用标志
	/// @param [in ] imageLayout 图像布局
	void LoadFromFile(
		std::string filename,
		VkFormat format,
		VulkanDevice* device,
		VkQueue copyQueue,
		VkImageUsageFlags imageUsageFlags = VK_IMAGE_USAGE_SAMPLED_BIT,
		VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	/// @brief 从缓冲区加载
	/// @param [in ] buffer 缓冲区
	/// @param [in ] bufferSize 缓冲区大小
	/// @param [in ] width 纹理宽
	/// @param [in ] height 纹理高
	/// @param [in ] device 物理设备指针
	/// @param [in ] copyQueue 拷贝队列
	/// @param [in ] imageUsageFlags 图像使用标志
	/// @param [in ] imageLayout 图像布局
	void LoadFromBuffer(
		void* buffer,
		VkDeviceSize bufferSize,
		VkFormat format,
		uint32_t width,
		uint32_t height,
		VulkanDevice* device,
		VkQueue copyQueue,
		VkFilter filter = VK_FILTER_LINEAR,
		VkImageUsageFlags imageUsageFlags = VK_IMAGE_USAGE_SAMPLED_BIT,
		VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
};


// 立方体纹理贴图
class DLLEXPORTCLASS TextureCubeMap : public VulkanTexture
{
public:
	/// @brief 从文件加载
	void LoadFromFile(
		std::string filename,
		VkFormat format,
		VulkanDevice* device,
		VkQueue copyQueue,
		VkImageUsageFlags imageUsageFlags = VK_IMAGE_USAGE_SAMPLED_BIT,
		VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
};