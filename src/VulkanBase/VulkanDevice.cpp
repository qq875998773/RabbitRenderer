#include "VulkanDevice.h"


VulkanDevice::VulkanDevice(VkPhysicalDevice physicalDevice)
{
	assert(physicalDevice);
	this->physicalDevice = physicalDevice;

	// �洢�����豸�����ԡ����ܡ����ƺ����ԣ��Թ��Ժ�ʹ��
	// �豸���Ի��������ƺ�ϡ������
	vkGetPhysicalDeviceProperties(physicalDevice, &properties);
	// ʹ��ǰ,Ӧͨ��ʾ����鹦��
	vkGetPhysicalDeviceFeatures(physicalDevice, &features);
	// �ڴ����Ծ������ڴ������ֻ�����
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);
	// ���������ԣ������ڴ����豸ʱ��������Ķ���
	uint32_t queueFamilyCount;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
	assert(queueFamilyCount > 0);
	queueFamilyProperties.resize(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilyProperties.data());
}

VulkanDevice::~VulkanDevice()
{
	// �ͷ������
	if (commandPool) 
	{
		vkDestroyCommandPool(logicalDevice, commandPool, nullptr);
	}
	// �ͷ��߼��豸
	if (logicalDevice) 
	{
		vkDestroyDevice(logicalDevice, nullptr);
	}
}

uint32_t VulkanDevice::GetMemoryType(uint32_t typeBits, VkMemoryPropertyFlags properties, VkBool32* memTypeFound)
{
	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
	{
		if ((typeBits & 1) == 1)
		{
			if ((memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
			{
				if (memTypeFound)
				{
					*memTypeFound = true;
				}
				return i;
			}
		}
		typeBits >>= 1;
	}

	if (memTypeFound)
	{
		*memTypeFound = false;
		return 0;
	}
	else
	{
		throw std::runtime_error("�Ҳ���ƥ����ڴ�����");
	}
}

uint32_t VulkanDevice::GetQueueFamilyIndex(VkQueueFlagBits queueFlags)
{
	// ����ר�ö���
	// ����֧�ּ��㵫��֧��ͼ�εĶ���������
	if (queueFlags & VK_QUEUE_COMPUTE_BIT)
	{
		for (uint32_t i = 0; i < static_cast<uint32_t>(queueFamilyProperties.size()); i++)
		{
			if ((queueFamilyProperties[i].queueFlags & queueFlags) && ((queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0))
			{
				return i;
				break;
			}
		}
	}

	// ���������������ͣ�����������ڵ����ļ�����У����ص�һ��������֧������ı�־
	for (uint32_t i = 0; i < static_cast<uint32_t>(queueFamilyProperties.size()); i++)
	{
		if (queueFamilyProperties[i].queueFlags & queueFlags)
		{
			return i;
			break;
		}
	}

	throw std::runtime_error("�Ҳ���ƥ��Ķ���������");
}

VkResult VulkanDevice::CreateLogicalDevice(VkPhysicalDeviceFeatures enabledFeatures, std::vector<const char*> enabledExtensions, VkQueueFlags requestedQueueTypes)
{
	// ��Ҫ�ڴ����߼��豸ʱ��������Ķ���
	// ����Vulkanʵ�ֵĶ���ϵ�����ò�ͬ��������Ӧ�ó��򣬿����е��鷳��
	// ����ͬ�Ķ�������
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};

	// ��ȡ����Ķ��������͵Ķ���������
	// ע�⣺�������ܻ��ص�������ȡ����ʵ��
	const float defaultQueuePriority(0.0f);

	// ͼ�ζ���
	if (requestedQueueTypes & VK_QUEUE_GRAPHICS_BIT)
	{
		queueFamilyIndices.graphics = GetQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT);
		VkDeviceQueueCreateInfo queueInfo{};
		queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueInfo.queueFamilyIndex = queueFamilyIndices.graphics;
		queueInfo.queueCount = 1;
		queueInfo.pQueuePriorities = &defaultQueuePriority;
		queueCreateInfos.push_back(queueInfo);
	}
	else
	{
		queueFamilyIndices.graphics = VK_NULL_HANDLE;
	}

	// ר�ü������
	if (requestedQueueTypes & VK_QUEUE_COMPUTE_BIT)
	{
		queueFamilyIndices.compute = GetQueueFamilyIndex(VK_QUEUE_COMPUTE_BIT);
		if (queueFamilyIndices.compute != queueFamilyIndices.graphics) // ���������������ͬ
		{
			// ������������һ�����д�����Ϣ
			VkDeviceQueueCreateInfo queueInfo{};
			queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueInfo.queueFamilyIndex = queueFamilyIndices.compute;
			queueInfo.queueCount = 1;
			queueInfo.pQueuePriorities = &defaultQueuePriority;
			queueCreateInfos.push_back(queueInfo);
		}
	}
	else // ���������������ͬ
	{
		// ʹ����ͬ�Ķ���
		queueFamilyIndices.compute = queueFamilyIndices.graphics;
	}

	// �����߼��豸��ʾ
	std::vector<const char*> deviceExtensions(enabledExtensions);
	deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());;
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
	deviceCreateInfo.pEnabledFeatures = &enabledFeatures;

	if (deviceExtensions.size() > 0)
	{
		deviceCreateInfo.enabledExtensionCount = (uint32_t)deviceExtensions.size();
		deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
	}

	VkResult result = vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &logicalDevice);

	if (result == VK_SUCCESS)
	{
		commandPool = CreateCommandPool(queueFamilyIndices.graphics);
	}

	this->enabledFeatures = enabledFeatures;

	return result;
}

VkResult VulkanDevice::CreateBuffer(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size, VkBuffer* buffer, VkDeviceMemory* memory, void* data)
{
	// �������������
	VkBufferCreateInfo bufferCreateInfo{};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.usage = usageFlags;
	bufferCreateInfo.size = size;
	bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	VK_CHECK_RESULT(vkCreateBuffer(logicalDevice, &bufferCreateInfo, nullptr, buffer));

	// �������ݻ�����������ڴ�
	VkMemoryRequirements memReqs;
	VkMemoryAllocateInfo memAlloc{};
	memAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	vkGetBufferMemoryRequirements(logicalDevice, *buffer, &memReqs);
	memAlloc.allocationSize = memReqs.size;
	// �����ʺϻ��������Ե��ڴ���������
	memAlloc.memoryTypeIndex = GetMemoryType(memReqs.memoryTypeBits, memoryPropertyFlags);
	VK_CHECK_RESULT(vkAllocateMemory(logicalDevice, &memAlloc, nullptr, memory));

	// ����Ѵ���ָ�򻺳������ݵ�ָ�룬��������������
	if (data != nullptr)
	{
		void* mapped;
		VK_CHECK_RESULT(vkMapMemory(logicalDevice, *memory, 0, size, 0, &mapped));
		memcpy(mapped, data, size);
		// ���δ��������һ���ԣ������ֶ�ˢ��ʹд��ɼ�
		if ((memoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) == 0)
		{
			VkMappedMemoryRange mappedRange{};
			mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
			mappedRange.memory = *memory;
			mappedRange.offset = 0;
			mappedRange.size = size;
			vkFlushMappedMemoryRanges(logicalDevice, 1, &mappedRange);
		}
		vkUnmapMemory(logicalDevice, *memory);
	}

	// ���ڴ渽�ӵ�����������
	VK_CHECK_RESULT(vkBindBufferMemory(logicalDevice, *buffer, *memory, 0));

	return VK_SUCCESS;
}

VkCommandPool VulkanDevice::CreateCommandPool(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags createFlags)
{
	VkCommandPoolCreateInfo cmdPoolInfo = {};
	cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cmdPoolInfo.queueFamilyIndex = queueFamilyIndex;
	cmdPoolInfo.flags = createFlags;
	VkCommandPool cmdPool;
	VK_CHECK_RESULT(vkCreateCommandPool(logicalDevice, &cmdPoolInfo, nullptr, &cmdPool));
	return cmdPool;
}

VkCommandBuffer VulkanDevice::CreateCommandBuffer(VkCommandBufferLevel level, bool begin)
{
	VkCommandBufferAllocateInfo cmdBufAllocateInfo{};
	cmdBufAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmdBufAllocateInfo.commandPool = commandPool;
	cmdBufAllocateInfo.level = level;
	cmdBufAllocateInfo.commandBufferCount = 1;

	VkCommandBuffer cmdBuffer;
	VK_CHECK_RESULT(vkAllocateCommandBuffers(logicalDevice, &cmdBufAllocateInfo, &cmdBuffer));

	// ��cmdBuffer��¼���������
	if (begin)
	{
		VkCommandBufferBeginInfo commandBufferBI{};
		commandBufferBI.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		VK_CHECK_RESULT(vkBeginCommandBuffer(cmdBuffer, &commandBufferBI));
	}

	return cmdBuffer;
}

void VulkanDevice::BeginCommandBuffer(VkCommandBuffer commandBuffer)
{
	VkCommandBufferBeginInfo commandBufferBI{};
	commandBufferBI.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	VK_CHECK_RESULT(vkBeginCommandBuffer(commandBuffer, &commandBufferBI));
}

void VulkanDevice::FlushCommandBuffer(VkCommandBuffer commandBuffer, VkQueue queue, bool free)
{
	VK_CHECK_RESULT(vkEndCommandBuffer(commandBuffer));

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	// ����fence��ȷ��������������ִ��
	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	VkFence fence;
	VK_CHECK_RESULT(vkCreateFence(logicalDevice, &fenceInfo, nullptr, &fence));

	// �ύ������
	VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, fence));
	// �ȴ�fence����,���������ִ�е��ź�
	VK_CHECK_RESULT(vkWaitForFences(logicalDevice, 1, &fence, VK_TRUE, 100000000000));

	vkDestroyFence(logicalDevice, fence, nullptr);

	if (free)
	{
		vkFreeCommandBuffers(logicalDevice, commandPool, 1, &commandBuffer);
	}
}