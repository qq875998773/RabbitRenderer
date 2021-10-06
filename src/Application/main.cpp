#include "VulkanPBR.h"
#include "VulkanObjModel.h"

// vulankPBR��Ⱦʵ��
VulkanPBR* VulkanPBRRender;

/// @brief ������Ϣ
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (VulkanPBRRender != NULL)
	{
		VulkanPBRRender->HandleMessages(hWnd, uMsg, wParam, lParam);
	}
	return (DefWindowProc(hWnd, uMsg, wParam, lParam));
}

/// @brief ������
/// @param [in ] hInstance ����ʵ��
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	//ImportModel test;
	////test.LoadModel("G:/c++/RabbitRenderer/data/models/OBJ/classroom/classroom.obj");
	//test.LoadModel("G:/c++/RabbitRenderer/data/models/glTF/BrainStem/glTF/BrainStem.gltf");

	VulkanPBRRender = new VulkanPBR();					// �½�vulkanPBRʵ��
	VulkanPBRRender->InitVulkan();						// ��ʼ��vulkan
	VulkanPBRRender->SetupWindow(hInstance, WndProc);	// ���ô���
	VulkanPBRRender->Prepare();							// ׼����Ⱦ
	VulkanPBRRender->RenderLoop();						// ��Ⱦѭ��
	delete(VulkanPBRRender);							// �˳���Ⱦ
	return 0;
}