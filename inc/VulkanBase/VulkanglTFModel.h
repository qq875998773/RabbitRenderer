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


// �ڴ˴����Ĵ�ֵ�����ڶ�����ɫ���н��и���
#define MAX_NUM_JOINTS 128u

/// @brief ����gltf 2.0ģ��
namespace vkglTF
{
	struct Node;

	// ���
	struct BoundingBox 
	{
		glm::vec3			min;
		glm::vec3			max;
		bool				valid = false;

		BoundingBox();
		BoundingBox(glm::vec3 min, glm::vec3 max);
		BoundingBox getAABB(glm::mat4 m);
	};

	// ���������
	struct TextureSampler 
	{
		VkFilter				magFilter;
		VkFilter				minFilter;
		VkSamplerAddressMode	addressModeU;
		VkSamplerAddressMode	addressModeV;
		VkSamplerAddressMode	addressModeW;
	};

	// ����
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

	// ����
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

	// ��ʼ����(ģ��ԭʼ��Ϣ)
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

	// ����
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

	// ��Ƥ����
	struct Skin 
	{
		std::string				name;
		Node*					skeletonRoot = nullptr;
		std::vector<glm::mat4>	inverseBindMatrices;
		std::vector<Node*>		joints;
	};

	// �ڵ�
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

	// ������Ϣ
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

	// ����������
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

	// ����
	struct Animation 
	{
		std::string						name;
		std::vector<AnimationSampler>	samplers;
		std::vector<AnimationChannel>	channels;
		float							start = std::numeric_limits<float>::max();
		float							end = std::numeric_limits<float>::min();
	};

	// ģ��
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

		/// @brief ����
		void destroy(VkDevice device);
		/// @brief ���ؽڵ�
		void loadNode(vkglTF::Node* parent, const tinygltf::Node& node, uint32_t nodeIndex, const tinygltf::Model& model, std::vector<uint32_t>& indexBuffer, std::vector<Vertex>& vertexBuffer, float globalscale);
		/// @brief ������Ƥ
		void loadSkins(tinygltf::Model& gltfModel);
		/// @brief ��������
		void loadTextures(tinygltf::Model& gltfModel, VulkanDevice* device, VkQueue transferQueue);
		/// @brief ��ȡvkѭ��ģʽ
		VkSamplerAddressMode getVkWrapMode(int32_t wrapMode);
		/// @brief ��ȡvk����ģʽ
		VkFilter getVkFilterMode(int32_t filterMode);
		/// @brief �������������
		void loadTextureSamplers(tinygltf::Model& gltfModel);
		/// @brief ���ز���
		void loadMaterials(tinygltf::Model& gltfModel);
		/// @brief ���ض���
		void loadAnimations(tinygltf::Model& gltfModel);
		/// @brief ����ģ��
		void loadFromFile(std::string filename, VulkanDevice* device, VkQueue transferQueue, float scale = 1.0f);
		/// @brief ���ƽڵ�
		void drawNode(Node* node, VkCommandBuffer commandBuffer);
		/// @brief ����
		void draw(VkCommandBuffer commandBuffer);
		/// @brief �������
		void calculateBoundingBox(Node* node, Node* parent);
		/// @brief ��ȡ�����ߴ�
		void getSceneDimensions();
		/// @brief ���ö���
		void updateAnimation(uint32_t index, float time);
		/// @brief ���ҽ��
		Node* findNode(Node* parent, uint32_t index);
		/// @brief ͨ���������ҽڵ�
		Node* nodeFromIndex(uint32_t index);
	};
}