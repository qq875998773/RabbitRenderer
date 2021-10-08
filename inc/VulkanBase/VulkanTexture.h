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


/// @brief vulkan����
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

	/// @brief ����������
	void UpdateDescriptor();

	/// @brief ����
	void Destroy();
};


/// @brief vulkan2d����
class DLLEXPORTCLASS VulkanTexture2D : public VulkanTexture
{
public:
	/// @brief ���ļ�����
	/// @param [in ] filename �ļ�·��
	/// @param [in ] format ��ʽ
	/// @param [in ] device �����豸ָ��
	/// @param [in ] copyQueue ��������
	/// @param [in ] imageUsageFlags ͼ��ʹ�ñ�־
	/// @param [in ] imageLayout ͼ�񲼾�
	void LoadFromFile(
		std::string filename,
		VkFormat format,
		VulkanDevice* device,
		VkQueue copyQueue,
		VkImageUsageFlags imageUsageFlags = VK_IMAGE_USAGE_SAMPLED_BIT,
		VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	/// @brief �ӻ���������
	/// @param [in ] buffer ������
	/// @param [in ] bufferSize ��������С
	/// @param [in ] width �����
	/// @param [in ] height �����
	/// @param [in ] device �����豸ָ��
	/// @param [in ] copyQueue ��������
	/// @param [in ] imageUsageFlags ͼ��ʹ�ñ�־
	/// @param [in ] imageLayout ͼ�񲼾�
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


// ������������ͼ
class DLLEXPORTCLASS TextureCubeMap : public VulkanTexture
{
public:
	/// @brief ���ļ�����
	void LoadFromFile(
		std::string filename,
		VkFormat format,
		VulkanDevice* device,
		VkQueue copyQueue,
		VkImageUsageFlags imageUsageFlags = VK_IMAGE_USAGE_SAMPLED_BIT,
		VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
};