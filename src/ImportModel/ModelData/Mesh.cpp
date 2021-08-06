#include "ModelData/Mesh.h"

Mesh::Mesh(VulkanDevice* device, glm::mat4 matrix)
{
	this->device = device;
	this->uniformBlock.matrix = matrix;
	VK_CHECK_RESULT(device->CreateBuffer(
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		sizeof(uniformBlock),
		&uniformBuffer.buffer,
		&uniformBuffer.memory,
		&uniformBlock));
	VK_CHECK_RESULT(vkMapMemory(device->logicalDevice, uniformBuffer.memory, 0, sizeof(uniformBlock), 0, &uniformBuffer.mapped));
	uniformBuffer.descriptor = { uniformBuffer.buffer, 0, sizeof(uniformBlock) };
};

Mesh::~Mesh()
{
	vkDestroyBuffer(device->logicalDevice, uniformBuffer.buffer, nullptr);
	vkFreeMemory(device->logicalDevice, uniformBuffer.memory, nullptr);
	for (Primitive* p : primitives)
	{
		delete p;
	}
}

void Mesh::SetBoundingBox(glm::vec3 min, glm::vec3 max)
{
	bb.min = min;
	bb.max = max;
	bb.valid = true;
}