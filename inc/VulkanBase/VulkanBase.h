#pragma once

#pragma comment(linker, "/subsystem:windows")
#include <windows.h>
#include <fcntl.h>
#include <io.h>

#include <iostream>
#include <chrono>
#include <sys/stat.h>
#include <string>
#include <sstream>
#include <array>
#include <numeric>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include "imgui/imgui.h"

#include "vulkan/vulkan.h"

#include "macros.h"
#include "camera.h"
#include "keycodes.hpp"

#include "VulkanDevice.h"
#include "VulkanSwapChain.h"


/// @brief ��ɫ�����
struct ColorOrDepth
{
	VkImage			image;
	VkImageView		view;
	VkDeviceMemory	memory;
};

/// @brief �����Ŀ��
struct MultisampleTarget
{
	ColorOrDepth color;
	ColorOrDepth depth;
};

/// @brief ����
struct Settings
{
	bool					validation = false;						// �Ƿ���֤
	bool					fullscreen = false;						// �Ƿ�ȫ��
	bool					vsync = false;							// �Ƿ�˺��
	bool					multiSampling = true;					// ���ز���
	VkSampleCountFlagBits	sampleCount = VK_SAMPLE_COUNT_4_BIT;	// ���ò�����
};

/// @brief ���ģ��
struct DepthStencil
{
	VkImage					image;
	VkDeviceMemory			mem;
	VkImageView				view;
};

/// @brief ��갴��
struct MouseButtons
{
	bool left = false;		// ������
	bool right = false;		// ����Ҽ�
	bool middle = false;	// ����м�
};

/// @brief vulkan��Ⱦ����
class VulkanBase
{
public: 
	static std::vector<const char*>			args;					// �����в���
	bool									prepared = false;		// ׼����Ⱦ
	uint32_t								width = 1280;			// ���ڿ�
	uint32_t								height = 720;			// ���ڸ�
	float									frameTimer = 1.0f;		// ֡ʱ��
	Camera									camera;					// �����
	glm::vec2								mousePos;				// ���λ��
	bool									paused = false;			// ֹͣˢ��
	uint32_t								lastFPS = 0;			// ֡��
	Settings								settings;				// ��Ⱦ����
	DepthStencil							depthStencil;			// ���ģ��
	MouseButtons							mouseButtons;			// ��갴��
	// ����ϵͳ�ض�����
	HWND									window;					// ���ھ��
	HINSTANCE								windowInstance;			// ����ʵ��

public:
	VulkanBase();
	virtual ~VulkanBase();

	/// @brief ���ô���
	/// @param [in ] hinstance ����ʵ��
	/// @param [in ] wndproc ������Ϣ
	HWND SetupWindow(HINSTANCE hinstance, WNDPROC wndproc);
	/// @brief ������Ϣ����
	/// @param [in ] hWnd ���ھ��
	/// @param [in ] wParam ͨ�������洢С����Ϣ���磬��־
	/// @param [in ] lParam ͨ�����ڴ洢��Ϣ����Ķ���
	void HandleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	/// @brief ��ʼ��vulkan
	void InitVulkan();

	/// @brief ����vulkanʵ��
	virtual VkResult CreateInstance(bool enableValidation);
	/// @brief ��Ⱦ
	virtual void Render() = 0;
	/// @brief �����С���� todo: ��ʱΪ��
	virtual void WindowResized() = 0;
	/// @brief ����֡������
	virtual void SetupFrameBuffer();
	/// @brief ׼����Ⱦ
	virtual void Prepare();

	/// @Brief ��ʼ��������
	void InitSwapchain();
	/// @brief ���ý�����
	void SetupSwapChain();

	/// @brief ��Ⱦѭ��
	void RenderLoop();
	/// @brief ��Ⱦ֡
	void RenderFrame();
private:
	float									m_fFpsTimer = 0.0f;					// ֡��ʱ��
	uint32_t								m_iFrameCounter = 0;				// ֡������
	uint32_t								m_iDestWidth;						// ���ĵĴ��ڿ�
	uint32_t								m_iDestHeight;						// ���ĵĴ��ڸ�
	bool									m_bResizing = false;				// ���ڴ�С�ı�
	PFN_vkCreateDebugReportCallbackEXT		m_vkCreateDebugReportCallback;		// ����������Ϣ�ص�
	PFN_vkDestroyDebugReportCallbackEXT		m_vkDestroyDebugReportCallback;		// ���ٵ�����Ϣ�ص�
	VkDebugReportCallbackEXT				debugReportCallback;				// ������Ϣ�ص�
	MultisampleTarget						multisampleTarget;					// �����Ŀ��
private:
	/// @brief ����ƶ��¼�
	void HandleMouseMove(int32_t x, int32_t y);
protected:
	VkInstance								instance;							// vulkanʵ��
	VkPhysicalDevice						physicalDevice;						// �����豸
	VkPhysicalDeviceProperties				deviceProperties;					// �豸����
	VkPhysicalDeviceFeatures				deviceFeatures;						// �豸����
	VkPhysicalDeviceMemoryProperties		deviceMemoryProperties;				// �豸�ڴ�����
	VkDevice								device;								// vk�߼��豸
	VulkanDevice*							vulkanDevice;						// ѡ��������豸��Ϣ
	VkQueue									queue;								// ����
	VkFormat								depthFormat;						// ��ȸ�ʽ
	VkCommandPool							cmdPool;							// ָ���
	VkRenderPass							renderPass;							// ��Ⱦ����
	std::vector<VkFramebuffer>				frameBuffers;						// ֡������
	uint32_t								currentBuffer = 0;					// ��ǰ������
	VkDescriptorPool						descriptorPool;						// ��������
	VkPipelineCache							pipelineCache;						// ���߻���
	VulkanSwapChain							swapChain;							// ������
	std::string								title = "Rabbit-Renderer";			// vk����̧ͷ
	std::string								name = "RabbitRenderer";			// vkӦ�ó���ע����
protected:
	/// @brief ���ڴ�С�ı䴦��
	void WindowResize();
};
