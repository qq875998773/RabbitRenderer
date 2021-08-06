#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <gli/gli.hpp>
#include <glm/gtx/string_cast.hpp>

//#include "Primitive.h"
//#include "BoundingBox.h"
#include "Node.h"

class Node;

/// @brief √…∆§
class Skin
{
public:
	std::string				name;
	Node*					skeletonRoot = nullptr;
	std::vector<glm::mat4>	inverseBindMatrices;
	std::vector<Node*>		joints;
};