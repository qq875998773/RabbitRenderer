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
#include "VulkanSwapChain.hpp"


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
	bool					validation = false;
	bool					fullscreen = false;
	bool					vsync = false;
	bool					multiSampling = true;
	VkSampleCountFlagBits	sampleCount = VK_SAMPLE_COUNT_4_BIT;
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


class VulkanExampleBase
{
private:
	float									fpsTimer = 0.0f;
	uint32_t								frameCounter = 0;
	uint32_t								destWidth;
	uint32_t								destHeight;
	bool									resizing = false;
	PFN_vkCreateDebugReportCallbackEXT		vkCreateDebugReportCallback;
	PFN_vkDestroyDebugReportCallbackEXT		vkDestroyDebugReportCallback;
	VkDebugReportCallbackEXT				debugReportCallback;
	MultisampleTarget						multisampleTarget;
private:
	void handleMouseMove(int32_t x, int32_t y);
protected:
	VkInstance								instance;
	VkPhysicalDevice						physicalDevice;
	VkPhysicalDeviceProperties				deviceProperties;
	VkPhysicalDeviceFeatures				deviceFeatures;
	VkPhysicalDeviceMemoryProperties		deviceMemoryProperties;
	VkDevice								device;
	VulkanDevice*							vulkanDevice;
	VkQueue									queue;
	VkFormat								depthFormat;
	VkCommandPool							cmdPool;
	VkRenderPass							renderPass;
	std::vector<VkFramebuffer>				frameBuffers;
	uint32_t								currentBuffer = 0;
	VkDescriptorPool						descriptorPool;
	VkPipelineCache							pipelineCache;
	VulkanSwapChain							swapChain;
	std::string								title = "Vulkan Renderer";
	std::string								name = "vulkanRenderer";
protected:
	void windowResize();
public: 
	static std::vector<const char*>			args;
	bool									prepared = false;		// ׼����Ⱦ
	uint32_t								width = 1280;
	uint32_t								height = 720;
	float									frameTimer = 1.0f;
	Camera									camera;					// �����
	glm::vec2								mousePos;
	bool									paused = false;			// ֹͣˢ��
	uint32_t								lastFPS = 0;
	Settings								settings;
	DepthStencil							depthStencil;			// ���ģ��
	MouseButtons							mouseButtons;			// ��갴��

	// ����ϵͳ�ض�����
	HWND									window;					// ���ھ��
	HINSTANCE								windowInstance;			// ����ʵ��

public:
	VulkanExampleBase();
	virtual ~VulkanExampleBase();

	/// @brief ���ô���
	/// @param [in ] hinstance ����ʵ��
	/// @param [in ] wndproc ������Ϣ
	HWND setupWindow(HINSTANCE hinstance, WNDPROC wndproc);
	/// @brief ��Ϣ����
	/// @param [in ] hWnd ���ھ��
	/// @param [in ] wParam ͨ�������洢С����Ϣ���磬��־
	/// @param [in ] lParam ͨ�����ڴ洢��Ϣ����Ķ���
	void handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	/// @brief ��ʼ��vulkan
	void initVulkan();

	/// @brief ����vulkanʵ��
	virtual VkResult createInstance(bool enableValidation);
	/// @brief ��Ⱦ
	virtual void render() = 0;
	/// @brief �����С���� todo: ��ʱΪ��
	virtual void windowResized();
	/// @brief ����֡������
	virtual void setupFrameBuffer();
	/// @brief ׼����Ⱦ
	virtual void prepare();

	/// @Brief ��ʼ��������
	void initSwapchain();
	/// @brief ���ý�����
	void setupSwapChain();

	/// @brief ��Ⱦѭ��
	void renderLoop();
	/// @brief ��Ⱦ֡
	void renderFrame();
};
