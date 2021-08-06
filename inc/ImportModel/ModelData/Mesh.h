#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <gli/gli.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Primitive.h"
#include "BoundingBox.h"

#include "vulkan/vulkan.h"

// 如此处更改必需在顶点着色器中进行对应更改
#define MAX_NUM_JOINTS 128u


/// @beief 统一缓冲区
struct UniformBuffer
{
	VkBuffer				buffer;
	VkDeviceMemory			memory;
	VkDescriptorBufferInfo	descriptor;
	VkDescriptorSet			descriptorSet;
	void*					mapped;
};

/// @brief 统一块
struct UniformBlock
{
	glm::mat4	matrix;
	glm::mat4	jointMatrix[MAX_NUM_JOINTS]{};
	float		jointcount{ 0 };
};


/// @brief 网格
class Mesh
{
public:
	Mesh(VulkanDevice* device, glm::mat4 matrix);
	~Mesh();

public:
	VulkanDevice*				device;
	std::vector<Primitive*>		primitives;
	BoundingBox					bb;
	BoundingBox					aabb;
	UniformBuffer				uniformBuffer;
	UniformBlock				uniformBlock;

public:
	void SetBoundingBox(glm::vec3 min, glm::vec3 max);
};