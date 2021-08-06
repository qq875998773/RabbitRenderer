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

// ��˴����ı����ڶ�����ɫ���н��ж�Ӧ����
#define MAX_NUM_JOINTS 128u


/// @beief ͳһ������
struct UniformBuffer
{
	VkBuffer				buffer;
	VkDeviceMemory			memory;
	VkDescriptorBufferInfo	descriptor;
	VkDescriptorSet			descriptorSet;
	void*					mapped;
};

/// @brief ͳһ��
struct UniformBlock
{
	glm::mat4	matrix;
	glm::mat4	jointMatrix[MAX_NUM_JOINTS]{};
	float		jointcount{ 0 };
};


/// @brief ����
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