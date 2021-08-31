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


/// @brief 颜色或深度
struct ColorOrDepth
{
	VkImage			image;
	VkImageView		view;
	VkDeviceMemory	memory;
};

/// @brief 多采样目标
struct MultisampleTarget
{
	ColorOrDepth color;
	ColorOrDepth depth;
};

/// @brief 设置
struct Settings
{
	bool					validation = false;						// 是否验证
	bool					fullscreen = false;						// 是否全屏
	bool					vsync = false;							// 是否抗撕裂
	bool					multiSampling = true;					// 多重采样
	VkSampleCountFlagBits	sampleCount = VK_SAMPLE_COUNT_4_BIT;	// 设置采样数
};

/// @brief 深度模板
struct DepthStencil
{
	VkImage					image;
	VkDeviceMemory			mem;
	VkImageView				view;
};

/// @brief 鼠标按键
struct MouseButtons
{
	bool left = false;		// 鼠标左键
	bool right = false;		// 鼠标右键
	bool middle = false;	// 鼠标中键
};

/// @brief vulkan渲染基类
class VulkanBase
{
public: 
	static std::vector<const char*>			args;					// 命令行参数
	bool									prepared = false;		// 准备渲染
	uint32_t								width = 1280;			// 窗口宽
	uint32_t								height = 720;			// 窗口高
	float									frameTimer = 1.0f;		// 帧时间
	Camera									camera;					// 摄像机
	glm::vec2								mousePos;				// 鼠标位置
	bool									paused = false;			// 停止刷新
	uint32_t								lastFPS = 0;			// 帧率
	Settings								settings;				// 渲染设置
	DepthStencil							depthStencil;			// 深度模板
	MouseButtons							mouseButtons;			// 鼠标按键
	// 操作系统特定对象
	HWND									window;					// 窗口句柄
	HINSTANCE								windowInstance;			// 窗口实例

public:
	VulkanBase();
	virtual ~VulkanBase();

	/// @brief 设置窗口
	/// @param [in ] hinstance 窗口实例
	/// @param [in ] wndproc 窗口消息
	HWND SetupWindow(HINSTANCE hinstance, WNDPROC wndproc);
	/// @brief 窗口消息处理
	/// @param [in ] hWnd 窗口句柄
	/// @param [in ] wParam 通常用来存储小段信息，如，标志
	/// @param [in ] lParam 通常用于存储消息所需的对象
	void HandleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	/// @brief 初始化vulkan
	void InitVulkan();

	/// @brief 创建vulkan实例
	virtual VkResult CreateInstance(bool enableValidation);
	/// @brief 渲染
	virtual void Render() = 0;
	/// @brief 窗体大小调整 todo: 暂时为空
	virtual void WindowResized() = 0;
	/// @brief 设置帧缓冲区
	virtual void SetupFrameBuffer();
	/// @brief 准备渲染
	virtual void Prepare();

	/// @Brief 初始化交换链
	void InitSwapchain();
	/// @brief 设置交换链
	void SetupSwapChain();

	/// @brief 渲染循环
	void RenderLoop();
	/// @brief 渲染帧
	void RenderFrame();
private:
	float									m_fFpsTimer = 0.0f;					// 帧率时间
	uint32_t								m_iFrameCounter = 0;				// 帧计数器
	uint32_t								m_iDestWidth;						// 更改的窗口宽
	uint32_t								m_iDestHeight;						// 更改的窗口高
	bool									m_bResizing = false;				// 窗口大小改变
	PFN_vkCreateDebugReportCallbackEXT		m_vkCreateDebugReportCallback;		// 创建调试消息回调
	PFN_vkDestroyDebugReportCallbackEXT		m_vkDestroyDebugReportCallback;		// 销毁调试消息回调
	VkDebugReportCallbackEXT				debugReportCallback;				// 调试消息回调
	MultisampleTarget						multisampleTarget;					// 多采样目标
private:
	/// @brief 鼠标移动事件
	void HandleMouseMove(int32_t x, int32_t y);
protected:
	VkInstance								instance;							// vulkan实例
	VkPhysicalDevice						physicalDevice;						// 物理设备
	VkPhysicalDeviceProperties				deviceProperties;					// 设备属性
	VkPhysicalDeviceFeatures				deviceFeatures;						// 设备功能
	VkPhysicalDeviceMemoryProperties		deviceMemoryProperties;				// 设备内存属性
	VkDevice								device;								// vk逻辑设备
	VulkanDevice*							vulkanDevice;						// 选择的物理设备信息
	VkQueue									queue;								// 队列
	VkFormat								depthFormat;						// 深度格式
	VkCommandPool							cmdPool;							// 指令池
	VkRenderPass							renderPass;							// 渲染过程
	std::vector<VkFramebuffer>				frameBuffers;						// 帧缓冲区
	uint32_t								currentBuffer = 0;					// 当前缓冲区
	VkDescriptorPool						descriptorPool;						// 描述符池
	VkPipelineCache							pipelineCache;						// 管线缓存
	VulkanSwapChain							swapChain;							// 交换链
	std::string								title = "Rabbit-Renderer";			// vk程序抬头
	std::string								name = "RabbitRenderer";			// vk应用程序注册名
protected:
	/// @brief 窗口大小改变处理
	void WindowResize();
};
