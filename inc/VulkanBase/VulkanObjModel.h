#pragma once

#include "VulkanBaseDef.h"

#include <string>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

/// @brief 解析obj模型
class DLLEXPORTCLASS ImportModel
{
public:
	ImportModel();
	~ImportModel();
public:
	/// @brief assimp加载模型
	void LoadModel(const std::string& filePath);
private:

};
