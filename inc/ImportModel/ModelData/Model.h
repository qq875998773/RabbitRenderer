//#pragma once
//
//#include <iostream>
//#include <stdlib.h>
//#include <string>
//#include <fstream>
//#include <vector>
//
//#include "vulkan/vulkan.h"
//
//#define GLM_FORCE_RADIANS
//#define GLM_FORCE_DEPTH_ZERO_TO_ONE
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <gli/gli.hpp>
//#include <glm/gtx/string_cast.hpp>
//
//#include "VulkanDevice.h"
//
//#include "Texture.h"
//#include "Node.h"
//#include "Animation.hpp"
//
//
///// @brief ����
//struct Vertex
//{
//	glm::vec3 pos;
//	glm::vec3 normal;
//	glm::vec2 uv0;
//	glm::vec2 uv1;
//	glm::vec4 joint0;
//	glm::vec4 weight0;
//};
//
///// @brief ���㻺����
//struct Vertices
//{
//	VkBuffer		buffer = VK_NULL_HANDLE;
//	VkDeviceMemory	memory;
//};
//
///// @brief ����
//struct Indices
//{
//	int				count;
//	VkBuffer		buffer = VK_NULL_HANDLE;
//	VkDeviceMemory	memory;
//};
//
///// @brief �ߴ�
//struct Dimensions
//{
//	glm::vec3 min = glm::vec3(FLT_MAX);
//	glm::vec3 max = glm::vec3(-FLT_MAX);
//};
//
//
///// @brief ģ����
//class Model
//{
//public:
//	VulkanDevice*					device;
//	Vertices						vertices;
//	Indices							indices;
//	glm::mat4						aabb;
//	std::vector<Node*>				nodes;
//	std::vector<Node*>				linearNodes;
//	std::vector<Skin*>				skins;
//	std::vector<Texture>			textures;
//	std::vector<TextureSampler>		textureSamplers;
//	std::vector<Material>			materials;
//	std::vector<Animation>			animations;
//	std::vector<std::string>		extensions;
//	Dimensions						dimensions;
//
//public:
//	/// @brief ����
//	void Destroy(VkDevice device);
//
//	/// @brief ���ؽڵ�
//	void LoadNode(Node* parent, const tinygltf::Node& node, uint32_t nodeIndex, const tinygltf::Model& model, std::vector<uint32_t>& indexBuffer, std::vector<Vertex>& vertexBuffer, float globalscale);
//
//	/// @brief ������Ƥ
//	void LoadSkins(tinygltf::Model& gltfModel);
//
//	/// @brief ��������
//	void LoadTextures(tinygltf::Model& gltfModel, VulkanDevice* device, VkQueue transferQueue);
//
//	/// @brief ��ȡѭ��ģʽ
//	VkSamplerAddressMode GetVkWrapMode(int32_t wrapMode);
//
//	/// @brief ��ȡ����ģʽ
//	VkFilter GetVkFilterMode(int32_t filterMode);
//
//	/// @brief �������������
//	void LoadTextureSamplers(tinygltf::Model& gltfModel);
//
//	/// @beief ���ز���
//	void LoadMaterials(tinygltf::Model& gltfModel);
//
//	/// @brief ���ض���
//	void LoadAnimations(tinygltf::Model& gltfModel);
//
//	/// @brief ����Model�ļ�(Ӧ�ĳ�assimp)
//	void LoadFromFile(std::string filename, VulkanDevice* device, VkQueue transferQueue, float scale = 1.0f);
//
//	/// @brief ���ƽڵ�
//	void DrawNode(Node* node, VkCommandBuffer commandBuffer);
//
//	/// @brief ����
//	void Draw(VkCommandBuffer commandBuffer);
//
//	/// @brief �������
//	void CalculateBoundingBox(Node* node, Node* parent);
//
//	/// @brief ��ȡ�������
//	void GetSceneDimensions();
//
//	/// @brief ���¶���
//	void UpdateAnimation(uint32_t index, float time);
//
//	/// @brief ���ҽڵ�
//	Node* FindNode(Node* parent, uint32_t index);
//
//	/// @brief ͨ���������ҽڵ�
//	Node* NodeFromIndex(uint32_t index);
//};