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


/// @brief ������
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
	/// @brief ����������
	void Create(VulkanDevice* device, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size, bool map = true);

	/// @brief ����
	void Destroy();

	void Map();

	/// @brief ����map
	void Unmap();

	/// @brief ˢ�»�����
	void Flush(VkDeviceSize size = VK_WHOLE_SIZE);
};

class DLLEXPORTCLASS VulkanUtils
{
public:
	/// @brief ����shader
	/// @param [in ] device �豸
	/// @param [in ] filename ��ɫ���ű��ļ���ַ
	/// @param [in ] stage ��ɫ����־λ(��ɫ������)
	static VkPipelineShaderStageCreateInfo LoadShader(VkDevice device, std::string filename, VkShaderStageFlagBits stage);

	/// @brief ��ȡĿ¼(����ͼƬ��Դ)
	/// @param [in ] directory �ļ���·��
	/// @param [in ] pattern �ļ���׺��(����.ktx)
	/// @param [out] filelist ������Դ�ļ�����Ӧ����Դ·������
	/// @param [in ] recursive �Ƿ�ݹ����ļ���
	static void ReadDirectory(const std::string& directory, const std::string& pattern, std::map<std::string, std::string>& filelist, bool recursive);
};