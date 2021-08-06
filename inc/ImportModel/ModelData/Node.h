#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <gli/gli.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Primitive.h"
#include "BoundingBox.h"
#include "Mesh.h"
#include "Skin.hpp"


///// @brief ��Ƥ
//struct Skin
//{
//public:
//	std::string				name;
//	Node* skeletonRoot = nullptr;
//	std::vector<glm::mat4>	inverseBindMatrices;
//	std::vector<Node*>		joints;
//};


class Node
{
public:
	~Node();

public:
	/*/// @brief ��Ƥ(��Ƥ�ͽڵ���Ի���Ƕ��,����д������)
	struct Skin
	{
		std::string				name;
		Node*					skeletonRoot = nullptr;
		std::vector<glm::mat4>	inverseBindMatrices;
		std::vector<Node*>		joints;
	};*/

	Node*					parent;
	uint32_t				index;
	std::vector<Node*>		children;
	glm::mat4				matrix;
	std::string				name;
	Mesh*					mesh;
	Skin*					skin;
	int32_t					skinIndex = -1;
	glm::vec3				translation{};
	glm::vec3				scale{ 1.0f };
	glm::quat				rotation{};
	BoundingBox				bvh;
	BoundingBox				aabb;

public:
	/// @brief ���ؾ���
	glm::mat4 LocalMatrix();

	/// @brief ��ȡ����
	glm::mat4 GetMatrix();

	/// @brief ����
	void Update();
};