#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <gli/gli.hpp>
#include <glm/gtx/string_cast.hpp>

#include "vulkan/vulkan.h"

#include "Texture.h"

/// @brief ͸����
enum AlphaMode 
{ 
	ALPHAMODE_OPAQUE,		// ��͸��
	ALPHAMODE_MASK,			// ��͸��
	ALPHAMODE_BLEND			// ͸��
};

struct TexCoordSets
{
	uint8_t baseColor = 0;
	uint8_t metallicRoughness = 0;
	uint8_t specularGlossiness = 0;
	uint8_t normal = 0;
	uint8_t occlusion = 0;
	uint8_t emissive = 0;
};

struct Extension
{
	Texture*	specularGlossinessTexture;
	Texture*	diffuseTexture;
	glm::vec4	diffuseFactor = glm::vec4(1.0f);
	glm::vec3	specularFactor = glm::vec3(0.0f);
};

struct PbrWorkflows
{
	bool metallicRoughness = true;
	bool specularGlossiness = false;
};


/// @brief ������
class Material
{
public:
	AlphaMode			alphaMode = ALPHAMODE_OPAQUE;		// Ĭ�ϲ�͸��
	float				alphaCutoff = 1.0f;
	float				metallicFactor = 1.0f;
	float				roughnessFactor = 1.0f;
	glm::vec4			baseColorFactor = glm::vec4(1.0f);
	glm::vec4			emissiveFactor = glm::vec4(1.0f);
	Texture*			baseColorTexture;
	Texture*			metallicRoughnessTexture;
	Texture*			normalTexture;
	Texture*			occlusionTexture;
	Texture*			emissiveTexture;
	TexCoordSets		texCoordSets;
	Extension			extension;
	PbrWorkflows		pbrWorkflows;						// PBR������
	VkDescriptorSet		descriptorSet = VK_NULL_HANDLE;
};