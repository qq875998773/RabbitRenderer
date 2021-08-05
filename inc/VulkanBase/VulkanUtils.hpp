#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include "vulkan/vulkan.h"
#include "VulkanDevice.h"


/// @brief ������
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
	/// @brief ����������
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

	/// @brief ����
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

	/// @brief ����map
	void Unmap()
	{
		if (mapped)
		{
			vkUnmapMemory(device, memory);
			mapped = nullptr;
		}
	}

	/// @brief ˢ�»�����
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
	/// @brief ����shader
	/// @param [in ] device �豸
	/// @param [in ] filename ��ɫ���ű��ļ���ַ
	/// @param [in ] stage ��ɫ����־λ(��ɫ������)
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

	/// @brief ��ȡĿ¼(����ͼƬ��Դ)
	/// @param [in ] directory �ļ���·��
	/// @param [in ] pattern �ļ���׺��(����.ktx)
	/// @param [out] filelist ������Դ�ļ�����Ӧ����Դ·������
	/// @param [in ] recursive �Ƿ�ݹ����ļ���
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