#include "VulkanObjModel.h"

ImportModel::ImportModel()
{
}

ImportModel::~ImportModel()
{
}

void ImportModel::LoadModel(const std::string& filePath)
{
	Assimp::Importer import;
	import.ReadFile(filePath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

	const aiScene* scene = import.GetScene();
	if (scene == NULL)
	{
		// log
		return;
	}

	aiNode* node = scene->mRootNode;

	auto istexture = scene->HasTextures();
	auto aaa = 0;
}
