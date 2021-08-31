#pragma once

#include <string>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

/// @brief ����objģ��
class ImportModel
{
public:
	ImportModel();
	~ImportModel();
public:
	/// @brief assimp����ģ��
	void LoadModel(const std::string& filePath);
private:

};
