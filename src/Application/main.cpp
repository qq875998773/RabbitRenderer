#include "VulkanRender.h"
#include "VulkanObjModel.h"

#include "QtGui/QGuiApplication.h"
#include "QtGui/QVulkanInstance.h"
#include "QtCore/QLoggingCategory.h"
#include "UI/QVulkanWindow.h"

// vulankPBR��Ⱦʵ��
VulkanRender* VulkanPBRRender;

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

	VulkanPBRRender = new VulkanRender();				// �½�ʵ��
	VulkanPBRRender->InitVulkan();						// ��ʼ��vulkan
	VulkanPBRRender->SetupWindow(hInstance, WndProc);	// ���ô���
	VulkanPBRRender->Prepare();							// ׼����Ⱦ
	VulkanPBRRender->RenderLoop();						// ��Ⱦѭ��
	delete(VulkanPBRRender);							// �˳���Ⱦ
	return 0;
}


//Q_LOGGING_CATEGORY(lcVk, "qt.vulkan")
//
//int main(int argc, char* argv[])
//{
//    QGuiApplication app(argc, argv);
//
//    QLoggingCategory::setFilterRules(QStringLiteral("qt.vulkan=true"));
//
//    QVulkanInstance inst;
//
//#ifndef Q_OS_ANDROID
//    inst.setLayers(QByteArrayList() << "VK_LAYER_LUNARG_standard_validation");
//#else
//    inst.setLayers(QByteArrayList()
//        << "VK_LAYER_GOOGLE_threading"
//        << "VK_LAYER_LUNARG_parameter_validation"
//        << "VK_LAYER_LUNARG_object_tracker"
//        << "VK_LAYER_LUNARG_core_validation"
//        << "VK_LAYER_LUNARG_image"
//        << "VK_LAYER_LUNARG_swapchain"
//        << "VK_LAYER_GOOGLE_unique_objects");
//#endif
//
//    if (!inst.create())
//    {
//        qFatal("Failed to create Vulkan instance: %d", inst.errorCode());
//    }
//
//    VulkanWindow w;
//    w.setVulkanInstance(&inst);
//
//    inst.vkInstance();
//
//    auto khr = inst.surfaceForWindow(&w);
//
//    w.resize(1024, 768);
//    w.show();
//
//    return app.exec();
//}