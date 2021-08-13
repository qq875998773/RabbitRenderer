#include "VulkanObjModel.h"

ImportModel::ImportModel()
{
}

ImportModel::~ImportModel()
{
}

void ImportModel::LoadModel(const std::string& filePath)
{
	Assimp::Importer im;
	im.ReadFile(filePath, 1);

	auto scene = im.GetScene();
	auto istexture = scene->HasTextures();
	auto aaa = 0;
}
