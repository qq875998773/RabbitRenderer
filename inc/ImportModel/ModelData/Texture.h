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


/// @brief 纹理采样器
struct TextureSampler
{
	VkFilter magFilter;
	VkFilter minFilter;
	VkSamplerAddressMode addressModeU;
	VkSamplerAddressMode addressModeV;
	VkSamplerAddressMode addressModeW;
};


/// @brief 纹理类
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
	/// @brief 更新描述符
	void UpdateDescriptor();

	/// @brief 销毁
	void Destroy();

	// 从glTF图像加载纹理(todo: 需改成assimp)
	void FromglTfImage(tinygltf::Image& gltfimage, TextureSampler textureSampler, VulkanDevice* device, VkQueue copyQueue);
};