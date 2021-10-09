#include "VulkanRender.h"
#include "VulkanObjModel.h"

#include "QtGui/QGuiApplication.h"
#include "QtGui/QVulkanInstance.h"
#include "QtCore/QLoggingCategory.h"
#include "UI/QVulkanWindow.h"

#include "QtGui/QVulkanWindow.h"


// vulankPBR��Ⱦʵ��
VulkanRender* VulkanPBRRender;
//
///// @brief ������Ϣ
//LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	if (VulkanPBRRender != NULL)
//	{
//		VulkanPBRRender->HandleMessages(hWnd, uMsg, wParam, lParam);
//	}
//	return (DefWindowProc(hWnd, uMsg, wParam, lParam));
//}
//
///// @brief ������
///// @param [in ] hInstance ����ʵ��
//int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
//{
//	//ImportModel test;
//	////test.LoadModel("G:/c++/RabbitRenderer/data/models/OBJ/classroom/classroom.obj");
//	//test.LoadModel("G:/c++/RabbitRenderer/data/models/glTF/BrainStem/glTF/BrainStem.gltf");
//
//	VulkanPBRRender = new VulkanRender();				// �½�ʵ��
//	VulkanPBRRender->InitVulkan();						// ��ʼ��vulkan
//	VulkanPBRRender->SetupWindow(hInstance, WndProc);	// ���ô���
//	VulkanPBRRender->Prepare();							// ׼����Ⱦ
//	VulkanPBRRender->RenderLoop();						// ��Ⱦѭ��
//	delete(VulkanPBRRender);							// �˳���Ⱦ
//	return 0;
//}


int main(int argc, char* argv[])
{
	QGuiApplication app(argc, argv);


	QWindow* qWindow = new QWindow();
	auto hInstance =  (HINSTANCE)qWindow->winId();//(HWND)
	qWindow->resize(1024, 768);
	qWindow->show();

	//VulkanPBRRender = new VulkanRender();				// �½�ʵ��
	//VulkanPBRRender->InitVulkan();						// ��ʼ��vulkan
	//VulkanPBRRender->SetupWindow(hInstance);	        // ���ô��� todo: ���������ò���
	//VulkanPBRRender->Prepare();							// ׼����Ⱦ
	//VulkanPBRRender->RenderLoop();						// ��Ⱦѭ��
	//delete(VulkanPBRRender);							// �˳���Ⱦ

	return app.exec();
}