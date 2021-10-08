#pragma once

#include "VulkanBaseDef.h"

#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include "vulkan/vulkan.h"
#include "VulkanDevice.h"


/// @brief 缓冲区
struct DLLEXPORTCLASS Buffer
{
public:
	VkDevice					device;
	VkBuffer					buffer = VK_NULL_HANDLE;
	VkDeviceMemory				memory = VK_NULL_HANDLE;
	VkDescriptorBufferInfo		descriptor;
	int32_t						count = 0;
	void* mapped = nullptr;
public:
	/// @brief 创建缓冲区
	void Create(VulkanDevice* device, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size, bool map = true);

	/// @brief 销毁
	void Destroy();

	void Map();

	/// @brief 销毁map
	void Unmap();

	/// @brief 刷新缓冲区
	void Flush(VkDeviceSize size = VK_WHOLE_SIZE);
};

class DLLEXPORTCLASS VulkanUtils
{
public:
	/// @brief 加载shader
	/// @param [in ] device 设备
	/// @param [in ] filename 着色器脚本文件地址
	/// @param [in ] stage 着色器标志位(着色器类型)
	static VkPipelineShaderStageCreateInfo LoadShader(VkDevice device, std::string filename, VkShaderStageFlagBits stage);

	/// @brief 读取目录(纹理图片资源)
	/// @param [in ] directory 文件夹路径
	/// @param [in ] pattern 文件后缀名(例如.ktx)
	/// @param [out] filelist 纹理资源文件命对应的资源路径集合
	/// @param [in ] recursive 是否递归子文件夹
	static void ReadDirectory(const std::string& directory, const std::string& pattern, std::map<std::string, std::string>& filelist, bool recursive);
};