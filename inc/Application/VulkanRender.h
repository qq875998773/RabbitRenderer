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

/// @brief ������Ϣ
struct Textures
{
	TextureCubeMap		environmentCube;		// ������
	VulkanTexture2D		empty;
	VulkanTexture2D		lutBrdf;				// �߹�brdf
	TextureCubeMap		irradianceCube;			// �����ն�
	TextureCubeMap		prefilteredCube;		// Ԥ����
};

struct Models
{
	vkglTF::Model scene;		// ����
	vkglTF::Model skybox;		// ��պ�
};

/// @brief ͳһ����������
struct UniformBufferSet
{
	Buffer scene;		// ����
	Buffer skybox;		// ��պ�
	Buffer params;
};

/// @brief ͳһ�������������
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
	const std::string						assetpath = "../../data/";			// ��Դ·�� Ĭ��data�ļ���
	bool									rotateModel = false;
	glm::vec3								modelrot = glm::vec3(0.0f);
	glm::vec3								modelPos = glm::vec3(0.0f);
	PushConstBlockMaterial					pushConstBlockMaterial;
	std::map<std::string, std::string>		environments;						// ������(��պ���)
	std::string								selectedEnvironment = "papermill";	// ѡ������(Ĭ��papermill)
	int32_t									debugViewInputs = 0;
	int32_t									debugViewEquation = 0;

public:
	VulkanRender();
	~VulkanRender();
	
	/// @brief ��Ⱦ�ڵ�
	void RenderNode(vkglTF::Node* node, uint32_t cbIndex, vkglTF::Material::AlphaMode alphaMode);
	
	/// @brief ��¼�������
	void RecordCommandBuffers();
	
	/// @brief ������Դ
	void LoadAssets();

	/// @brief ���س���
	/// @param [in ] filename �����ļ�Ŀ¼
	void LoadScene(std::string filename);

	/// @brief ���ػ�����ͼ
	/// @param [in ] filename ������ԴĿ¼
	void LoadEnvironment(std::string filename);

	/// @brief ���ýڵ���������
	/// @param [in ] node �ڵ�ָ��
	void SetupNodeDescriptorSet(vkglTF::Node* node);

	/// @brief ����������
	void SetupDescriptors();

	/// @brief ׼����Ⱦ����
	void PreparePipelines();

	///	@brief ���ɽ��ֲڶ�(NdotV)�洢Ϊ���ұ��BRDF����ͼ
	void GenerateBRDFLUT();
	
	/// @brief ����PBR��������������ͼ����������
	/// @note ���ն�������ͼ
	/// @note Ԥ���˻�������ͼ
	void GenerateCubemaps();
	
	///	@brief ׼������ʼ��������ɫ��������ͳһ������
	void PrepareUniformBuffers();

	/// @brief ���°�����ɫ��������ͳһ������
	void UpdateUniformBuffers();
	
	/// @brief ������ɫ��ֵ����
	void UpdateParams();

	/// @brief �����С�仯
	virtual void WindowResized();

	/// @brief ��Ⱦ׼��
	virtual void Prepare();
	
	/// @brief ����ImGui�û�����
	//void UpdateOverlay();
	
	/// @brief ��Ⱦ
	virtual void Render();
};