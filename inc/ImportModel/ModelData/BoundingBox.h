#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <gli/gli.hpp>
#include <glm/gtx/string_cast.hpp>

/// @brief 外包类 todo:后续移到几何运算里
class BoundingBox
{
public:
	BoundingBox();
	BoundingBox(glm::vec3 min, glm::vec3 max);
	~BoundingBox();
public:
	glm::vec3	min;			// 最小值
	glm::vec3	max;			// 最大值
	bool		valid = false;	// 是否有效
public:
	/// @brief 计算AABB
	BoundingBox GetAABB(glm::mat4 m);
};