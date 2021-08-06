#pragma once

#include <vector>
#include <string>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <gli/gli.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Node.h"


enum PathType
{
	TRANSLATION,
	ROTATION,
	SCALE
};

enum InterpolationType
{
	LINEAR,		// ����
	STEP,
	CUBICSPLINE
};


/// @brief ������Ϣ
struct AnimationChannel
{
	PathType	path;
	Node*		node;
	uint32_t	samplerIndex;
};

/// @brief ����������
struct AnimationSampler
{
	InterpolationType		interpolation;
	std::vector<float>		inputs;
	std::vector<glm::vec4>	outputsVec4;
};


/// @brief ����
class Animation
{
public:
	std::string						name;
	std::vector<AnimationSampler>	samplers;
	std::vector<AnimationChannel>	channels;
	float							start		= std::numeric_limits<float>::max();
	float							end			= std::numeric_limits<float>::min();
};