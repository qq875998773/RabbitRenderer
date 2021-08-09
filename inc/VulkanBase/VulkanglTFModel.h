#pragma once

#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <vector>

#include "vulkan/vulkan.h"
#include "VulkanDevice.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <gli/gli.hpp>
#include <glm/gtx/string_cast.hpp>

#include "tiny_gltf.h"


// 在此处更改此值还需在顶点着色器中进行更改
#define MAX_NUM_JOINTS 128u

/// @brief 解析gltf 2.0模型
namespace vkglTF
{
	struct Node;

	// 外包
	struct BoundingBox 
	{
		glm::vec3			min;
		glm::vec3			max;
		bool				valid = false;

		BoundingBox();
		BoundingBox(glm::vec3 min, glm::vec3 max);
		BoundingBox getAABB(glm::mat4 m);
	};

	// 纹理采样器
	struct TextureSampler 
	{
		VkFilter				magFilter;
		VkFilter				minFilter;
		VkSamplerAddressMode	addressModeU;
		VkSamplerAddressMode	addressModeV;
		VkSamplerAddressMode	addressModeW;
	};

	// 纹理
	struct Texture 
	{
		VulkanDevice*			device;
		VkImage					image;
		VkImageLayout			imageLayout;
		VkDeviceMemory			deviceMemory;
		VkImageView				view;
		uint32_t				width;
		uint32_t				height;
		uint32_t				mipLevels;
		uint32_t				layerCount;
		VkDescriptorImageInfo	descriptor;
		VkSampler				sampler;

		void updateDescriptor();
		void destroy();
		// Load a texture from a glTF image (stored as vector of chars loaded via stb_image) and generate a full mip chaing for it
		void fromglTfImage(tinygltf::Image& gltfimage, TextureSampler textureSampler, VulkanDevice* device, VkQueue copyQueue);
	};

	// 材质
	struct Material 
	{
		enum AlphaMode
		{ 
			ALPHAMODE_OPAQUE,
			ALPHAMODE_MASK,
			ALPHAMODE_BLEND
		};

		AlphaMode			alphaMode = ALPHAMODE_OPAQUE;
		float				alphaCutoff = 1.0f;
		float				metallicFactor = 1.0f;
		float				roughnessFactor = 1.0f;
		glm::vec4			baseColorFactor = glm::vec4(1.0f);
		glm::vec4			emissiveFactor = glm::vec4(1.0f);
		vkglTF::Texture*	baseColorTexture;
		vkglTF::Texture*	metallicRoughnessTexture;
		vkglTF::Texture*	normalTexture;
		vkglTF::Texture*	occlusionTexture;
		vkglTF::Texture*	emissiveTexture;

		struct TexCoordSets 
		{
			uint8_t baseColor = 0;
			uint8_t metallicRoughness = 0;
			uint8_t specularGlossiness = 0;
			uint8_t normal = 0;
			uint8_t occlusion = 0;
			uint8_t emissive = 0;
		} texCoordSets;
		struct Extension 
		{
			vkglTF::Texture*	specularGlossinessTexture;
			vkglTF::Texture*	diffuseTexture;
			glm::vec4			diffuseFactor = glm::vec4(1.0f);
			glm::vec3			specularFactor = glm::vec3(0.0f);
		} extension;
		struct PbrWorkflows 
		{
			bool metallicRoughness = true;
			bool specularGlossiness = false;
		} pbrWorkflows;
		VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
	};

	// 初始数据(模型原始信息)
	struct Primitive 
	{
		uint32_t	firstIndex;
		uint32_t	indexCount;
		uint32_t	vertexCount;
		Material&	material;
		bool		hasIndices;
		BoundingBox bb;

		Primitive(uint32_t firstIndex, uint32_t indexCount, uint32_t vertexCount, Material& material);
		void setBoundingBox(glm::vec3 min, glm::vec3 max);
	};

	// 网格
	struct Mesh 
	{
		VulkanDevice*			device;
		std::vector<Primitive*> primitives;
		BoundingBox				bb;
		BoundingBox				aabb;
		struct UniformBuffer 
		{
			VkBuffer				buffer;
			VkDeviceMemory			memory;
			VkDescriptorBufferInfo	descriptor;
			VkDescriptorSet			descriptorSet;
			void*					mapped;
		} uniformBuffer;
		struct UniformBlock 
		{
			glm::mat4	matrix;
			glm::mat4	jointMatrix[MAX_NUM_JOINTS]{};
			float		jointcount{ 0 };
		} uniformBlock;

		Mesh(VulkanDevice* device, glm::mat4 matrix);
		~Mesh();
		void setBoundingBox(glm::vec3 min, glm::vec3 max);
	};

	// 蒙皮动画
	struct Skin 
	{
		std::string				name;
		Node*					skeletonRoot = nullptr;
		std::vector<glm::mat4>	inverseBindMatrices;
		std::vector<Node*>		joints;
	};

	// 节点
	struct Node 
	{
		Node*				parent;
		uint32_t			index;
		std::vector<Node*>	children;
		glm::mat4			matrix;
		std::string			name;
		Mesh*				mesh;
		Skin*				skin;
		int32_t				skinIndex = -1;
		glm::vec3			translation{};
		glm::vec3			scale{ 1.0f };
		glm::quat			rotation{};
		BoundingBox			bvh;
		BoundingBox			aabb;

		glm::mat4 localMatrix();
		glm::mat4 getMatrix();
		void update();
		~Node();
	};

	// 动画信息
	struct AnimationChannel 
	{
		enum PathType
		{
			TRANSLATION, 
			ROTATION, 
			SCALE
		};

		PathType	path;
		Node*		node;
		uint32_t	samplerIndex;
	};

	// 动画采样器
	struct AnimationSampler 
	{
		enum InterpolationType 
		{ 
			LINEAR, 
			STEP, 
			CUBICSPLINE 
		};

		InterpolationType		interpolation;
		std::vector<float>		inputs;
		std::vector<glm::vec4>	outputsVec4;
	};

	// 动画
	struct Animation 
	{
		std::string						name;
		std::vector<AnimationSampler>	samplers;
		std::vector<AnimationChannel>	channels;
		float							start = std::numeric_limits<float>::max();
		float							end = std::numeric_limits<float>::min();
	};

	// 模型
	struct Model 
	{
		VulkanDevice *device;

		struct Vertex 
		{
			glm::vec3 pos;
			glm::vec3 normal;
			glm::vec2 uv0;
			glm::vec2 uv1;
			glm::vec4 joint0;
			glm::vec4 weight0;
		};

		struct Vertices 
		{
			VkBuffer		buffer = VK_NULL_HANDLE;
			VkDeviceMemory	memory;
		} vertices;
		struct Indices 
		{
			int				count;
			VkBuffer		buffer = VK_NULL_HANDLE;
			VkDeviceMemory	memory;
		} indices;

		glm::mat4					aabb;
		std::vector<Node*>			nodes;
		std::vector<Node*>			linearNodes;
		std::vector<Skin*>			skins;
		std::vector<Texture>		textures;
		std::vector<TextureSampler> textureSamplers;
		std::vector<Material>		materials;
		std::vector<Animation>		animations;
		std::vector<std::string>	extensions;

		struct Dimensions 
		{
			glm::vec3 min = glm::vec3(FLT_MAX);
			glm::vec3 max = glm::vec3(-FLT_MAX);
		} dimensions;

		/// @brief 销毁
		void destroy(VkDevice device);
		/// @brief 加载节点
		void loadNode(vkglTF::Node* parent, const tinygltf::Node& node, uint32_t nodeIndex, const tinygltf::Model& model, std::vector<uint32_t>& indexBuffer, std::vector<Vertex>& vertexBuffer, float globalscale);
		/// @brief 加载蒙皮
		void loadSkins(tinygltf::Model& gltfModel);
		/// @brief 加载纹理
		void loadTextures(tinygltf::Model& gltfModel, VulkanDevice* device, VkQueue transferQueue);
		/// @brief 获取vk循环模式
		VkSamplerAddressMode getVkWrapMode(int32_t wrapMode);
		/// @brief 获取vk过滤模式
		VkFilter getVkFilterMode(int32_t filterMode);
		/// @brief 加载纹理采样器
		void loadTextureSamplers(tinygltf::Model& gltfModel);
		/// @brief 加载材质
		void loadMaterials(tinygltf::Model& gltfModel);
		/// @brief 加载动画
		void loadAnimations(tinygltf::Model& gltfModel);
		/// @brief 加载模型
		void loadFromFile(std::string filename, VulkanDevice* device, VkQueue transferQueue, float scale = 1.0f);
		/// @brief 绘制节点
		void drawNode(Node* node, VkCommandBuffer commandBuffer);
		/// @brief 绘制
		void draw(VkCommandBuffer commandBuffer);
		/// @brief 计算外包
		void calculateBoundingBox(Node* node, Node* parent);
		/// @brief 获取场景尺寸
		void getSceneDimensions();
		/// @brief 设置动画
		void updateAnimation(uint32_t index, float time);
		/// @brief 查找结点
		Node* findNode(Node* parent, uint32_t index);
		/// @brief 通过索引查找节点
		Node* nodeFromIndex(uint32_t index);
	};
}