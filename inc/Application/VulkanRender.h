#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <vector>
#include <chrono>
#include <map>
#include "algorithm"

#include <vulkan/vulkan.h>
#include "VulkanBase.h"
#include "VulkanTexture.h"
#include "VulkanglTFModel.h"
#include "VulkanUtils.h"
#include "ui.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/// @brief 纹理信息
struct Textures
{
	TextureCubeMap		environmentCube;		// 环境盒
	VulkanTexture2D		empty;
	VulkanTexture2D		lutBrdf;				// 高光brdf
	TextureCubeMap		irradianceCube;			// 辐射照度
	TextureCubeMap		prefilteredCube;		// 预过滤
};

struct Models
{
	vkglTF::Model scene;		// 场景
	vkglTF::Model skybox;		// 天空盒
};

/// @brief 统一缓冲区设置
struct UniformBufferSet
{
	Buffer scene;		// 场景
	Buffer skybox;		// 天空盒
	Buffer params;
};

/// @brief 统一缓冲区对象矩阵
struct UBOMatrices
{
	glm::mat4 projection;
	glm::mat4 model;
	glm::mat4 view;
	glm::vec3 camPos;
};

struct ShaderValuesParams
{
	glm::vec4	lightDir;
	float		exposure = 4.5f;
	float		gamma = 2.2f;
	float		prefilteredCubeMipLevels;
	float		scaleIBLAmbient = 1.0f;
	float		debugViewInputs = 0;
	float		debugViewEquation = 0;
};

struct Pipelines
{
	VkPipeline skybox;
	VkPipeline pbr;
	VkPipeline pbrAlphaBlend;
};

struct DescriptorSetLayouts
{
	VkDescriptorSetLayout scene;
	VkDescriptorSetLayout material;
	VkDescriptorSetLayout node;
};

struct DescriptorSets
{
	VkDescriptorSet scene;
	VkDescriptorSet skybox;
};

struct LightSource
{
	glm::vec3 color = glm::vec3(1.0f);
	glm::vec3 rotation = glm::vec3(75.0f, 40.0f, 0.0f);
};

enum PBRWorkflows 
{ 
	PBR_WORKFLOW_METALLIC_ROUGHNESS = 0,
	PBR_WORKFLOW_SPECULAR_GLOSINESS = 1
};

struct PushConstBlockMaterial
{
	glm::vec4	baseColorFactor;
	glm::vec4	emissiveFactor;
	glm::vec4	diffuseFactor;
	glm::vec4	specularFactor;
	float		workflow;
	int			colorTextureSet;
	int			physicalDescriptorTextureSet;
	int			normalTextureSet;
	int			occlusionTextureSet;
	int			emissiveTextureSet;
	float		metallicFactor;
	float		roughnessFactor;
	float		alphaMask;
	float		alphaMaskCutoff;
};


class VulkanRender : public VulkanBase
{
public:
	Textures								textures;
	Models									models;
	UBOMatrices								shaderValuesScene;
	UBOMatrices								shaderValuesSkybox;
	ShaderValuesParams						shaderValuesParams;
	VkPipelineLayout						pipelineLayout;
	Pipelines								pipelines;
	DescriptorSetLayouts					descriptorSetLayouts;
	std::vector<DescriptorSets>				descriptorSets;
	std::vector<VkCommandBuffer>			commandBuffers;
	std::vector<UniformBufferSet>			uniformBuffers;
	std::vector<VkFence>					waitFences;
	std::vector<VkSemaphore>				renderCompleteSemaphores;
	std::vector<VkSemaphore>				presentCompleteSemaphores;
	const uint32_t							renderAhead = 2;
	uint32_t								frameIndex = 0;
	int32_t									animationIndex = 0;
	float									animationTimer = 0.0f;
	bool									animate = true;
	bool									displayBackground = true;
	LightSource								lightSource;
	//UI*										ui;
	const std::string						assetpath = "../../data/";			// 资源路径 默认data文件夹
	bool									rotateModel = false;
	glm::vec3								modelrot = glm::vec3(0.0f);
	glm::vec3								modelPos = glm::vec3(0.0f);
	PushConstBlockMaterial					pushConstBlockMaterial;
	std::map<std::string, std::string>		environments;						// 环境集(天空盒子)
	std::string								selectedEnvironment = "papermill";	// 选定环境(默认papermill)
	int32_t									debugViewInputs = 0;
	int32_t									debugViewEquation = 0;

public:
	VulkanRender();
	~VulkanRender();
	
	/// @brief 渲染节点
	void RenderNode(vkglTF::Node* node, uint32_t cbIndex, vkglTF::Material::AlphaMode alphaMode);
	
	/// @brief 记录命令缓冲区
	void RecordCommandBuffers();
	
	/// @brief 加载资源
	void LoadAssets();

	/// @brief 加载场景
	/// @param [in ] filename 场景文件目录
	void LoadScene(std::string filename);

	/// @brief 加载环境贴图
	/// @param [in ] filename 环境资源目录
	void LoadEnvironment(std::string filename);

	/// @brief 设置节点描述符集
	/// @param [in ] node 节点指针
	void SetupNodeDescriptorSet(vkglTF::Node* node);

	/// @brief 设置描述符
	void SetupDescriptors();

	/// @brief 准备渲染管线
	void PreparePipelines();

	///	@brief 生成将粗糙度(NdotV)存储为查找表的BRDF集成图
	void GenerateBRDFLUT();
	
	/// @brief 用于PBR照明的立方体贴图的离线生成
	/// @note 辐照度立方体图
	/// @note 预过滤环境立方图
	void GenerateCubemaps();
	
	///	@brief 准备并初始化包含着色器参数的统一缓冲区
	void PrepareUniformBuffers();

	/// @brief 更新包含着色器参数的统一缓冲区
	void UpdateUniformBuffers();
	
	/// @brief 更新着色器值参数
	void UpdateParams();

	/// @brief 窗体大小变化
	virtual void WindowResized();

	/// @brief 渲染准备
	virtual void Prepare();
	
	/// @brief 更新ImGui用户界面
	//void UpdateOverlay();
	
	/// @brief 渲染
	virtual void Render();
};