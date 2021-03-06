#include "VulkanRender.h"
#include "VulkanObjModel.h"

// vulankPBR渲染实例
VulkanRender* VulkanPBRRender;

/// @brief 窗口消息
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (VulkanPBRRender != NULL)
	{
		VulkanPBRRender->HandleMessages(hWnd, uMsg, wParam, lParam);
	}
	return (DefWindowProc(hWnd, uMsg, wParam, lParam));
}

/// @brief 主函数
/// @param [in ] hInstance 窗口实例
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	//ImportModel test;
	////test.LoadModel("G:/c++/RabbitRenderer/data/models/OBJ/classroom/classroom.obj");
	//test.LoadModel("G:/c++/RabbitRenderer/data/models/glTF/BrainStem/glTF/BrainStem.gltf");

	VulkanPBRRender = new VulkanRender();				// 新建实例
	VulkanPBRRender->InitVulkan();						// 初始化vulkan
	VulkanPBRRender->SetupWindow(hInstance, WndProc);	// 设置窗体
	VulkanPBRRender->Prepare();							// 准备渲染
	VulkanPBRRender->RenderLoop();						// 渲染循环
	delete(VulkanPBRRender);							// 退出渲染
	return 0;
}