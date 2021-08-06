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
///// @brief 顶点
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
///// @brief 顶点缓冲区
//struct Vertices
//{
//	VkBuffer		buffer = VK_NULL_HANDLE;
//	VkDeviceMemory	memory;
//};
//
///// @brief 索引
//struct Indices
//{
//	int				count;
//	VkBuffer		buffer = VK_NULL_HANDLE;
//	VkDeviceMemory	memory;
//};
//
///// @brief 尺寸
//struct Dimensions
//{
//	glm::vec3 min = glm::vec3(FLT_MAX);
//	glm::vec3 max = glm::vec3(-FLT_MAX);
//};
//
//
///// @brief 模型类
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
//	/// @brief 销毁
//	void Destroy(VkDevice device);
//
//	/// @brief 加载节点
//	void LoadNode(Node* parent, const tinygltf::Node& node, uint32_t nodeIndex, const tinygltf::Model& model, std::vector<uint32_t>& indexBuffer, std::vector<Vertex>& vertexBuffer, float globalscale);
//
//	/// @brief 加载蒙皮
//	void LoadSkins(tinygltf::Model& gltfModel);
//
//	/// @brief 加载纹理
//	void LoadTextures(tinygltf::Model& gltfModel, VulkanDevice* device, VkQueue transferQueue);
//
//	/// @brief 获取循环模式
//	VkSamplerAddressMode GetVkWrapMode(int32_t wrapMode);
//
//	/// @brief 获取过滤模式
//	VkFilter GetVkFilterMode(int32_t filterMode);
//
//	/// @brief 加载纹理采样器
//	void LoadTextureSamplers(tinygltf::Model& gltfModel);
//
//	/// @beief 加载材质
//	void LoadMaterials(tinygltf::Model& gltfModel);
//
//	/// @brief 加载动画
//	void LoadAnimations(tinygltf::Model& gltfModel);
//
//	/// @brief 加载Model文件(应改成assimp)
//	void LoadFromFile(std::string filename, VulkanDevice* device, VkQueue transferQueue, float scale = 1.0f);
//
//	/// @brief 绘制节点
//	void DrawNode(Node* node, VkCommandBuffer commandBuffer);
//
//	/// @brief 绘制
//	void Draw(VkCommandBuffer commandBuffer);
//
//	/// @brief 计算外包
//	void CalculateBoundingBox(Node* node, Node* parent);
//
//	/// @brief 获取场景外包
//	void GetSceneDimensions();
//
//	/// @brief 更新动画
//	void UpdateAnimation(uint32_t index, float time);
//
//	/// @brief 查找节点
//	Node* FindNode(Node* parent, uint32_t index);
//
//	/// @brief 通过索引查找节点
//	Node* NodeFromIndex(uint32_t index);
//};