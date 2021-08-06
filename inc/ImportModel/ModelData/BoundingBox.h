#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <gli/gli.hpp>
#include <glm/gtx/string_cast.hpp>

/// @brief ����� todo:�����Ƶ�����������
class BoundingBox
{
public:
	BoundingBox();
	BoundingBox(glm::vec3 min, glm::vec3 max);
	~BoundingBox();
public:
	glm::vec3	min;			// ��Сֵ
	glm::vec3	max;			// ���ֵ
	bool		valid = false;	// �Ƿ���Ч
public:
	/// @brief ����AABB
	BoundingBox GetAABB(glm::mat4 m);
};