#include "VulkanRender.h"
#include "VulkanObjModel.h"

#include "QtGui/QGuiApplication.h"
#include "QtGui/QVulkanInstance.h"
#include "QtCore/QLoggingCategory.h"
#include "UI/QVulkanWindow.h"

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