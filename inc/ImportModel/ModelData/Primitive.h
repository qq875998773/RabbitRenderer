#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <gli/gli.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Material.hpp"
#include "BoundingBox.h"


/// @brief ģ�ͳ�ʼ��Ϣ��
/// @note ��¼��ʼ��š�����������������������ʡ������
class Primitive
{
public:
	Primitive(uint32_t firstIndex, uint32_t indexCount, uint32_t vertexCount, Material& material);
public:
	uint32_t		firstIndex;
	uint32_t		indexCount;
	uint32_t		vertexCount;
	Material&		material;
	bool			hasIndices;
	BoundingBox		bb;
public:
	void SetBoundingBox(glm::vec3 min, glm::vec3 max);
};