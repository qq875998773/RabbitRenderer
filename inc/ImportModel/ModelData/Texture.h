#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <gli/gli.hpp>
#include <glm/gtx/string_cast.hpp>

#include "vulkan/vulkan.h"

#include "tiny_gltf.h"

#include "VulkanDevice.h"


/// @brief ���������
struct TextureSampler
{
	VkFilter magFilter;
	VkFilter minFilter;
	VkSamplerAddressMode addressModeU;
	VkSamplerAddressMode addressModeV;
	VkSamplerAddressMode addressModeW;
};


/// @brief ������
class Texture
{
public:
	VulkanDevice*			device;
	VkImage					image;
	VkImageLayout			imageLayout;
	VkDeviceMemory			deviceMemory;
	VkImageView				view;
	uint32_t				width, height;
	uint32_t				mipLevels;
	uint32_t				layerCount;
	VkDescriptorImageInfo	descriptor;
	VkSampler				sampler;

public:
	/// @brief ����������
	void UpdateDescriptor();

	/// @brief ����
	void Destroy();

	// ��glTFͼ���������(todo: ��ĳ�assimp)
	void FromglTfImage(tinygltf::Image& gltfimage, TextureSampler textureSampler, VulkanDevice* device, VkQueue copyQueue);
};