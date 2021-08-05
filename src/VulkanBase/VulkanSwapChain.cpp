#include "VulkanSwapChain.h"


void VulkanSwapChain::InitSurface(void* platformHandle, void* platformWindow)
{
	VkResult err = VK_SUCCESS;

	// ��������ϵͳ�ض��ı���
	VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.hinstance = (HINSTANCE)platformHandle;
	surfaceCreateInfo.hwnd = (HWND)platformWindow;
	err = vkCreateWin32SurfaceKHR(m_instance, &surfaceCreateInfo, nullptr, &m_surface);

	if (err != VK_SUCCESS) 
	{
		std::cerr << "�޷���������!" << std::endl;
		exit(err);
	}

	// ��ȡ���õĶ���������
	uint32_t queueCount;
	vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueCount, NULL);
	assert(queueCount >= 1);

	std::vector<VkQueueFamilyProperties> queueProps(queueCount);
	vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueCount, queueProps.data());

	// ����ÿ���������˽����Ƿ�֧����ʾ��
	// ���Ҿ��е�ǰ֧�ֵĶ���
	// �������򴰿�ϵͳ��ʾ������ͼ��
	std::vector<VkBool32> supportsPresent(queueCount);
	for (uint32_t i = 0; i < queueCount; i++)
	{
		fpGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, i, m_surface, &supportsPresent[i]);
	}

	// �ڶ�������������ͼ�κ͵�ǰ����
	// ���Բ���ͬʱ֧����������������
	uint32_t graphicsQueueNodeIndex = UINT32_MAX;
	uint32_t presentQueueNodeIndex = UINT32_MAX;
	for (uint32_t i = 0; i < queueCount; i++)
	{
		if ((queueProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)
		{
			if (graphicsQueueNodeIndex == UINT32_MAX)
			{
				graphicsQueueNodeIndex = i;
			}

			if (supportsPresent[i] == VK_TRUE)
			{
				graphicsQueueNodeIndex = i;
				presentQueueNodeIndex = i;
				break;
			}
		}
	}
	if (presentQueueNodeIndex == UINT32_MAX)
	{
		// ���û��ͬʱ֧����ʾ��ͼ�εĶ���
		// ���Բ��ҵ����ĵ�ǰ����
		for (uint32_t i = 0; i < queueCount; ++i)
		{
			if (supportsPresent[i] == VK_TRUE)
			{
				presentQueueNodeIndex = i;
				break;
			}
		}
	}

	// ���δ�ҵ�ͼ�λ���ʾ���У����˳�
	if (graphicsQueueNodeIndex == UINT32_MAX || presentQueueNodeIndex == UINT32_MAX) 
	{
		std::cerr << "δ�ҵ�ͼ�κ�/����ʾ����!" << std::endl;
		exit(-1);
	}

	// todo : ��ӶԵ���ͼ�κ���ʾ���е�֧��
	if (graphicsQueueNodeIndex != presentQueueNodeIndex) {
		std::cerr << "�в�֧�ֵ�����ͼ�κ���ʾ����!" << std::endl;
		exit(-1);
	}

	queueNodeIndex = graphicsQueueNodeIndex;

	// ��ȡ֧�ֵ������ʽ�б�
	uint32_t formatCount;
	VK_CHECK_RESULT(fpGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &formatCount, NULL));
	assert(formatCount > 0);

	std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
	VK_CHECK_RESULT(fpGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &formatCount, surfaceFormats.data()));

	// ��������ʽ�б�ֻ��һ���ҵ���VK_FORMAT_UNDEFINED,������ΪVK_FORMAT_B8G8R8A8_UNORM
	if ((formatCount == 1) && (surfaceFormats[0].format == VK_FORMAT_UNDEFINED))
	{
		colorFormat = VK_FORMAT_B8G8R8A8_UNORM;
		colorSpace = surfaceFormats[0].colorSpace;
	}
	else
	{
		// ѭ�����ҿ��������ʽ�б�������Ƿ����VK_FORMAT_B8G8R8A8_UNORM
		bool found_B8G8R8A8_UNORM = false;
		for (auto&& surfaceFormat : surfaceFormats)
		{
			// ���� SRGB
			if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_SRGB)
			{
				colorFormat = surfaceFormat.format;
				colorSpace = surfaceFormat.colorSpace;
				found_B8G8R8A8_UNORM = true;
				break;
			}
			//if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM)
			//{
			//	colorFormat = surfaceFormat.format;
			//	colorSpace = surfaceFormat.colorSpace;
			//	found_B8G8R8A8_UNORM = true;
			//	break;
			//}
		}

		// ���VK_FORMAT_B8G8R8A8_UNORM�����ã���ѡ���һ�ֿ��õ���ɫ
		if (!found_B8G8R8A8_UNORM)
		{
			colorFormat = surfaceFormats[0].format;
			colorSpace = surfaceFormats[0].colorSpace;
		}
	}
}

void VulkanSwapChain::Connect(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device)
{
	this->m_instance = instance;
	this->m_physicalDevice = physicalDevice;
	this->m_device = device;
	GET_INSTANCE_PROC_ADDR(instance, GetPhysicalDeviceSurfaceSupportKHR);
	GET_INSTANCE_PROC_ADDR(instance, GetPhysicalDeviceSurfaceCapabilitiesKHR);
	GET_INSTANCE_PROC_ADDR(instance, GetPhysicalDeviceSurfaceFormatsKHR);
	GET_INSTANCE_PROC_ADDR(instance, GetPhysicalDeviceSurfacePresentModesKHR);
	GET_DEVICE_PROC_ADDR(device, CreateSwapchainKHR);
	GET_DEVICE_PROC_ADDR(device, DestroySwapchainKHR);
	GET_DEVICE_PROC_ADDR(device, GetSwapchainImagesKHR);
	GET_DEVICE_PROC_ADDR(device, AcquireNextImageKHR);
	GET_DEVICE_PROC_ADDR(device, QueuePresentKHR);
}

void VulkanSwapChain::Create(uint32_t* width, uint32_t* height, bool vsync)
{
	VkSwapchainKHR oldSwapchain = swapChain;

	// ��ȡ�����豸�������Ժ͸�ʽ
	VkSurfaceCapabilitiesKHR surfCaps;
	VK_CHECK_RESULT(fpGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice, m_surface, &surfCaps));

	// ��ȡ���õĵ�ǰģʽ
	uint32_t presentModeCount;
	VK_CHECK_RESULT(fpGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, m_surface, &presentModeCount, NULL));
	assert(presentModeCount > 0);

	std::vector<VkPresentModeKHR> presentModes(presentModeCount);
	VK_CHECK_RESULT(fpGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, m_surface, &presentModeCount, presentModes.data()));

	// ������(�߶�)��������ֵ0xFFFFFFFF��������Ĵ�С����swapchain����
	if (surfCaps.currentExtent.width == (uint32_t)-1)
	{
		// ��������Сδ���壬��ô�С������Ϊ������ͼ��Ĵ�С��
		extent.width = *width;
		extent.height = *height;
	}
	else
	{
		// ��������������С���򽻻�����С����ƥ��
		extent = surfCaps.currentExtent;
		*width = surfCaps.currentExtent.width;
		*height = surfCaps.currentExtent.height;
	}

	// Ϊ������ѡ��ǰģʽ

	// ���ݹ淶��VK_PRESENT_MODE_FIFO_KHR MODE����ʼ�մ���
	// ��ģʽ�ȴ���ֱ�հף���v-sync����
	VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;

	// �������v-sync
	if (!vsync)
	{
		// ��������ӳٷǵ�ǰģʽ
		for (size_t i = 0; i < presentModeCount; i++)
		{
			if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				swapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
				break;
			}
			if ((swapchainPresentMode != VK_PRESENT_MODE_MAILBOX_KHR) && (presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR))
			{
				swapchainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
			}
		}
	}

	// ȷ��ͼ�������
	uint32_t desiredNumberOfSwapchainImages = surfCaps.minImageCount + 1;
	if ((surfCaps.maxImageCount > 0) && (desiredNumberOfSwapchainImages > surfCaps.maxImageCount))
	{
		desiredNumberOfSwapchainImages = surfCaps.maxImageCount;
	}

	// ����surface�ı任
	VkSurfaceTransformFlagsKHR preTransform;
	if (surfCaps.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
	{
		// ����ת�任
		preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	}
	else
	{
		preTransform = surfCaps.currentTransform;
	}

	// ����֧�ֵĸ���alpha��ʽ�����������豸��֧��alpha��͸����
	VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	// ֻ��ѡ���һ�����õĸ���alpha��ʽ
	std::vector<VkCompositeAlphaFlagBitsKHR> compositeAlphaFlags =
	{
		VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
		VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
		VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
	};
	for (auto& compositeAlphaFlag : compositeAlphaFlags)
	{
		if (surfCaps.supportedCompositeAlpha & compositeAlphaFlag)
		{
			compositeAlpha = compositeAlphaFlag;
			break;
		}
	}

	VkSwapchainCreateInfoKHR swapchainCI = {};
	swapchainCI.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainCI.pNext = NULL;
	swapchainCI.surface = m_surface;
	swapchainCI.minImageCount = desiredNumberOfSwapchainImages;
	swapchainCI.imageFormat = colorFormat;
	swapchainCI.imageColorSpace = colorSpace;
	swapchainCI.imageExtent = { extent.width, extent.height };
	swapchainCI.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchainCI.preTransform = (VkSurfaceTransformFlagBitsKHR)preTransform;
	swapchainCI.imageArrayLayers = 1;
	swapchainCI.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapchainCI.queueFamilyIndexCount = 0;
	swapchainCI.pQueueFamilyIndices = NULL;
	swapchainCI.presentMode = swapchainPresentMode;
	swapchainCI.oldSwapchain = oldSwapchain;
	// ����clipped to VK_TRUE����ʵ�ַ�����������֮�����Ⱦ
	swapchainCI.clipped = VK_TRUE;
	swapchainCI.compositeAlpha = compositeAlpha;

	// ���֧�֣�Ϊswapchainͼ���blitting��������ʹ�ñ�־
	VkFormatProperties formatProps;
	vkGetPhysicalDeviceFormatProperties(m_physicalDevice, colorFormat, &formatProps);
	if ((formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_TRANSFER_SRC_BIT_KHR) || (formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_SRC_BIT))
	{
		swapchainCI.imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
	}

	VK_CHECK_RESULT(fpCreateSwapchainKHR(m_device, &swapchainCI, nullptr, &swapChain));

	// ������´������н������������پɽ�����
	// ��Ҳ��������пɳ��ֵ�ͼ��
	if (oldSwapchain != VK_NULL_HANDLE)
	{
		for (uint32_t i = 0; i < imageCount; i++)
		{
			vkDestroyImageView(m_device, buffers[i].view, nullptr);
		}
		fpDestroySwapchainKHR(m_device, oldSwapchain, nullptr);
	}
	VK_CHECK_RESULT(fpGetSwapchainImagesKHR(m_device, swapChain, &imageCount, NULL));

	// ��ȡ������ͼ��
	images.resize(imageCount);
	VK_CHECK_RESULT(fpGetSwapchainImagesKHR(m_device, swapChain, &imageCount, images.data()));

	// ��ȡ����ͼ���ͼ���ӿڵĽ�����������
	buffers.resize(imageCount);
	for (uint32_t i = 0; i < imageCount; i++)
	{
		VkImageViewCreateInfo colorAttachmentView = {};
		colorAttachmentView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		colorAttachmentView.pNext = NULL;
		colorAttachmentView.format = colorFormat;
		colorAttachmentView.components =
		{
			VK_COMPONENT_SWIZZLE_R,
			VK_COMPONENT_SWIZZLE_G,
			VK_COMPONENT_SWIZZLE_B,
			VK_COMPONENT_SWIZZLE_A
		};
		colorAttachmentView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		colorAttachmentView.subresourceRange.baseMipLevel = 0;
		colorAttachmentView.subresourceRange.levelCount = 1;
		colorAttachmentView.subresourceRange.baseArrayLayer = 0;
		colorAttachmentView.subresourceRange.layerCount = 1;
		colorAttachmentView.viewType = VK_IMAGE_VIEW_TYPE_2D;
		colorAttachmentView.flags = 0;

		buffers[i].image = images[i];

		colorAttachmentView.image = buffers[i].image;

		VK_CHECK_RESULT(vkCreateImageView(m_device, &colorAttachmentView, nullptr, &buffers[i].view));
	}
}

VkResult VulkanSwapChain::AcquireNextImage(VkSemaphore presentCompleteSemaphore, uint32_t* imageIndex)
{
	if (swapChain == VK_NULL_HANDLE) {
		return VK_ERROR_OUT_OF_DATE_KHR;
	}

	// ͨ����timeout����ΪUINT64_MAX��ʼ�յȴ���ֱ����ȡ��һ��ͼ����׳�ʵ�ʴ���
	// �����Ͳ��ش���VK_NOT_READY��
	return fpAcquireNextImageKHR(m_device, swapChain, UINT64_MAX, presentCompleteSemaphore, (VkFence)nullptr, imageIndex);
}

VkResult VulkanSwapChain::QueuePresent(VkQueue queue, uint32_t imageIndex, VkSemaphore waitSemaphore)
{
	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.pNext = NULL;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &swapChain;
	presentInfo.pImageIndices = &imageIndex;
	// ����Ƿ���ָ������ʾͼ��֮ǰ�ȴ��ĵȴ��ź���
	if (waitSemaphore != VK_NULL_HANDLE)
	{
		presentInfo.pWaitSemaphores = &waitSemaphore;
		presentInfo.waitSemaphoreCount = 1;
	}
	return fpQueuePresentKHR(queue, &presentInfo);
}

void VulkanSwapChain::Cleanup()
{
	if (swapChain != VK_NULL_HANDLE)
	{
		for (uint32_t i = 0; i < imageCount; i++)
		{
			vkDestroyImageView(m_device, buffers[i].view, nullptr);
		}
	}
	if (m_surface != VK_NULL_HANDLE)
	{
		fpDestroySwapchainKHR(m_device, swapChain, nullptr);
		vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
	}
	m_surface = VK_NULL_HANDLE;
	swapChain = VK_NULL_HANDLE;
}