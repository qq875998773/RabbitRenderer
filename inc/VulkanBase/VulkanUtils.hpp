#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include "vulkan/vulkan.h"
#include "VulkanDevice.h"


/// @brief 缓冲区
struct Buffer
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
	void Create(VulkanDevice* device, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size, bool map = true)
	{
		this->device = device->logicalDevice;
		device->CreateBuffer(usageFlags, memoryPropertyFlags, size, &buffer, &memory);
		descriptor = { buffer, 0, size };
		if (map)
		{
			VK_CHECK_RESULT(vkMapMemory(device->logicalDevice, memory, 0, size, 0, &mapped));
		}
	}

	/// @brief 销毁
	void Destroy()
	{
		if (mapped)
		{
			Unmap();
		}
		vkDestroyBuffer(device, buffer, nullptr);
		vkFreeMemory(device, memory, nullptr);
		buffer = VK_NULL_HANDLE;
		memory = VK_NULL_HANDLE;
	}

	void Map()
	{
		VK_CHECK_RESULT(vkMapMemory(device, memory, 0, VK_WHOLE_SIZE, 0, &mapped));
	}

	/// @brief 销毁map
	void Unmap()
	{
		if (mapped)
		{
			vkUnmapMemory(device, memory);
			mapped = nullptr;
		}
	}

	/// @brief 刷新缓冲区
	void Flush(VkDeviceSize size = VK_WHOLE_SIZE)
	{
		VkMappedMemoryRange mappedRange{};
		mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mappedRange.memory = memory;
		mappedRange.size = size;
		VK_CHECK_RESULT(vkFlushMappedMemoryRanges(device, 1, &mappedRange));
	}
};

class VulkanUtils
{
public:
	/// @brief 加载shader
	/// @param [in ] device 设备
	/// @param [in ] filename 着色器脚本文件地址
	/// @param [in ] stage 着色器标志位(着色器类型)
	static VkPipelineShaderStageCreateInfo LoadShader(VkDevice device, std::string filename, VkShaderStageFlagBits stage)
	{
		VkPipelineShaderStageCreateInfo shaderStage{};
		shaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStage.stage = stage;
		shaderStage.pName = "main";
		std::ifstream is("../../shaders/" + filename, std::ifstream::binary | std::ifstream::in | std::ifstream::ate);

		if (is.is_open())
		{
			size_t size = is.tellg();
			is.seekg(0, std::ios::beg);
			char* shaderCode = new char[size];
			is.read(shaderCode, size);
			is.close();
			assert(size > 0);
			VkShaderModuleCreateInfo moduleCreateInfo{};
			moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			moduleCreateInfo.codeSize = size;
			moduleCreateInfo.pCode = (uint32_t*)shaderCode;
			vkCreateShaderModule(device, &moduleCreateInfo, NULL, &shaderStage.module);
			delete[] shaderCode;
		}
		else
		{
			std::cerr << "Error: Could not open shader file \"" << filename << "\"" << std::endl;
			shaderStage.module = VK_NULL_HANDLE;
		}

		assert(shaderStage.module != VK_NULL_HANDLE);
		return shaderStage;
	}

	/// @brief 读取目录(纹理图片资源)
	/// @param [in ] directory 文件夹路径
	/// @param [in ] pattern 文件后缀名(例如.ktx)
	/// @param [out] filelist 纹理资源文件命对应的资源路径集合
	/// @param [in ] recursive 是否递归子文件夹
	static void ReadDirectory(const std::string& directory, const std::string& pattern, std::map<std::string, std::string>& filelist, bool recursive)
	{
		std::string searchpattern(directory + "/" + pattern);
		WIN32_FIND_DATA data;
		HANDLE hFind;
		if ((hFind = FindFirstFile(searchpattern.c_str(), &data)) != INVALID_HANDLE_VALUE)
		{
			do
			{
				std::string filename(data.cFileName);
				filename.erase(filename.find_last_of("."), std::string::npos);
				filelist[filename] = directory + "/" + data.cFileName;
			} while (FindNextFile(hFind, &data) != 0);
			FindClose(hFind);
		}
		if (recursive)
		{
			std::string dirpattern = directory + "/*";
			if ((hFind = FindFirstFile(dirpattern.c_str(), &data)) != INVALID_HANDLE_VALUE)
			{
				do
				{
					if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						char subdir[MAX_PATH];
						strcpy(subdir, directory.c_str());
						strcat(subdir, "/");
						strcat(subdir, data.cFileName);
						if ((strcmp(data.cFileName, ".") != 0) && (strcmp(data.cFileName, "..") != 0))
						{
							ReadDirectory(subdir, pattern, filelist, recursive);
						}
					}
				} while (FindNextFile(hFind, &data) != 0);
				FindClose(hFind);
			}
		}
	}
};