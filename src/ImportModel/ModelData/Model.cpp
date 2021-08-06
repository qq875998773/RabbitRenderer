//#include "ModelData/Model.h"
//
//void Model::Destroy(VkDevice device)
//{
//	if (vertices.buffer != VK_NULL_HANDLE)
//	{
//		vkDestroyBuffer(device, vertices.buffer, nullptr);
//		vkFreeMemory(device, vertices.memory, nullptr);
//		vertices.buffer = VK_NULL_HANDLE;
//	}
//
//	if (indices.buffer != VK_NULL_HANDLE)
//	{
//		vkDestroyBuffer(device, indices.buffer, nullptr);
//		vkFreeMemory(device, indices.memory, nullptr);
//		indices.buffer = VK_NULL_HANDLE;
//	}
//
//	for (auto texture : textures)
//	{
//		texture.Destroy();
//	}
//
//	textures.resize(0);
//	textureSamplers.resize(0);
//	for (auto node : nodes)
//	{
//		delete node;
//	}
//	materials.resize(0);
//	animations.resize(0);
//	nodes.resize(0);
//	linearNodes.resize(0);
//	extensions.resize(0);
//	for (auto skin : skins)
//	{
//		delete skin;
//	}
//	skins.resize(0);
//};
//
//void Model::LoadNode(Node* parent, const tinygltf::Node& node, uint32_t nodeIndex, const tinygltf::Model& model, std::vector<uint32_t>& indexBuffer, std::vector<Vertex>& vertexBuffer, float globalscale)
//{
//	Node* newNode = new Node();
//	newNode->index = nodeIndex;
//	newNode->parent = parent;
//	newNode->name = node.name;
//	newNode->skinIndex = node.skin;
//	newNode->matrix = glm::mat4(1.0f);
//
//	// Generate local node matrix
//	glm::vec3 translation = glm::vec3(0.0f);
//	if (node.translation.size() == 3)
//	{
//		translation = glm::make_vec3(node.translation.data());
//		newNode->translation = translation;
//	}
//	glm::mat4 rotation = glm::mat4(1.0f);
//	if (node.rotation.size() == 4)
//	{
//		glm::quat q = glm::make_quat(node.rotation.data());
//		newNode->rotation = glm::mat4(q);
//	}
//	glm::vec3 scale = glm::vec3(1.0f);
//	if (node.scale.size() == 3)
//	{
//		scale = glm::make_vec3(node.scale.data());
//		newNode->scale = scale;
//	}
//	if (node.matrix.size() == 16)
//	{
//		newNode->matrix = glm::make_mat4x4(node.matrix.data());
//	};
//
//	// Node with children
//	if (node.children.size() > 0)
//	{
//		for (size_t i = 0; i < node.children.size(); i++)
//		{
//			LoadNode(newNode, model.nodes[node.children[i]], node.children[i], model, indexBuffer, vertexBuffer, globalscale);
//		}
//	}
//
//	// Node contains mesh data
//	if (node.mesh > -1)
//	{
//		const tinygltf::Mesh mesh = model.meshes[node.mesh];
//		Mesh* newMesh = new Mesh(device, newNode->matrix);
//		for (size_t j = 0; j < mesh.primitives.size(); j++)
//		{
//			const tinygltf::Primitive& primitive = mesh.primitives[j];
//			uint32_t indexStart = static_cast<uint32_t>(indexBuffer.size());
//			uint32_t vertexStart = static_cast<uint32_t>(vertexBuffer.size());
//			uint32_t indexCount = 0;
//			uint32_t vertexCount = 0;
//			glm::vec3 posMin{};
//			glm::vec3 posMax{};
//			bool hasSkin = false;
//			bool hasIndices = primitive.indices > -1;
//			// Vertices
//			{
//				const float* bufferPos = nullptr;
//				const float* bufferNormals = nullptr;
//				const float* bufferTexCoordSet0 = nullptr;
//				const float* bufferTexCoordSet1 = nullptr;
//				const void* bufferJoints = nullptr;
//				const float* bufferWeights = nullptr;
//
//				int posByteStride;
//				int normByteStride;
//				int uv0ByteStride;
//				int uv1ByteStride;
//				int jointByteStride;
//				int weightByteStride;
//
//				int jointComponentType;
//
//				// Position attribute is required
//				assert(primitive.attributes.find("POSITION") != primitive.attributes.end());
//
//				const tinygltf::Accessor& posAccessor = model.accessors[primitive.attributes.find("POSITION")->second];
//				const tinygltf::BufferView& posView = model.bufferViews[posAccessor.bufferView];
//				bufferPos = reinterpret_cast<const float*>(&(model.buffers[posView.buffer].data[posAccessor.byteOffset + posView.byteOffset]));
//				posMin = glm::vec3(posAccessor.minValues[0], posAccessor.minValues[1], posAccessor.minValues[2]);
//				posMax = glm::vec3(posAccessor.maxValues[0], posAccessor.maxValues[1], posAccessor.maxValues[2]);
//				vertexCount = static_cast<uint32_t>(posAccessor.count);
//				posByteStride = posAccessor.ByteStride(posView) ? (posAccessor.ByteStride(posView) / sizeof(float)) : tinygltf::GetTypeSizeInBytes(TINYGLTF_TYPE_VEC3);
//
//				if (primitive.attributes.find("NORMAL") != primitive.attributes.end())
//				{
//					const tinygltf::Accessor& normAccessor = model.accessors[primitive.attributes.find("NORMAL")->second];
//					const tinygltf::BufferView& normView = model.bufferViews[normAccessor.bufferView];
//					bufferNormals = reinterpret_cast<const float*>(&(model.buffers[normView.buffer].data[normAccessor.byteOffset + normView.byteOffset]));
//					normByteStride = normAccessor.ByteStride(normView) ? (normAccessor.ByteStride(normView) / sizeof(float)) : tinygltf::GetTypeSizeInBytes(TINYGLTF_TYPE_VEC3);
//				}
//
//				if (primitive.attributes.find("TEXCOORD_0") != primitive.attributes.end())
//				{
//					const tinygltf::Accessor& uvAccessor = model.accessors[primitive.attributes.find("TEXCOORD_0")->second];
//					const tinygltf::BufferView& uvView = model.bufferViews[uvAccessor.bufferView];
//					bufferTexCoordSet0 = reinterpret_cast<const float*>(&(model.buffers[uvView.buffer].data[uvAccessor.byteOffset + uvView.byteOffset]));
//					uv0ByteStride = uvAccessor.ByteStride(uvView) ? (uvAccessor.ByteStride(uvView) / sizeof(float)) : tinygltf::GetTypeSizeInBytes(TINYGLTF_TYPE_VEC2);
//				}
//				if (primitive.attributes.find("TEXCOORD_1") != primitive.attributes.end())
//				{
//					const tinygltf::Accessor& uvAccessor = model.accessors[primitive.attributes.find("TEXCOORD_1")->second];
//					const tinygltf::BufferView& uvView = model.bufferViews[uvAccessor.bufferView];
//					bufferTexCoordSet1 = reinterpret_cast<const float*>(&(model.buffers[uvView.buffer].data[uvAccessor.byteOffset + uvView.byteOffset]));
//					uv1ByteStride = uvAccessor.ByteStride(uvView) ? (uvAccessor.ByteStride(uvView) / sizeof(float)) : tinygltf::GetTypeSizeInBytes(TINYGLTF_TYPE_VEC2);
//				}
//
//				// Skinning
//				// Joints
//				if (primitive.attributes.find("JOINTS_0") != primitive.attributes.end())
//				{
//					const tinygltf::Accessor& jointAccessor = model.accessors[primitive.attributes.find("JOINTS_0")->second];
//					const tinygltf::BufferView& jointView = model.bufferViews[jointAccessor.bufferView];
//					bufferJoints = &(model.buffers[jointView.buffer].data[jointAccessor.byteOffset + jointView.byteOffset]);
//					jointComponentType = jointAccessor.componentType;
//					jointByteStride = jointAccessor.ByteStride(jointView) ? (jointAccessor.ByteStride(jointView) / tinygltf::GetComponentSizeInBytes(jointComponentType)) : tinygltf::GetTypeSizeInBytes(TINYGLTF_TYPE_VEC4);
//				}
//
//				if (primitive.attributes.find("WEIGHTS_0") != primitive.attributes.end())
//				{
//					const tinygltf::Accessor& weightAccessor = model.accessors[primitive.attributes.find("WEIGHTS_0")->second];
//					const tinygltf::BufferView& weightView = model.bufferViews[weightAccessor.bufferView];
//					bufferWeights = reinterpret_cast<const float*>(&(model.buffers[weightView.buffer].data[weightAccessor.byteOffset + weightView.byteOffset]));
//					weightByteStride = weightAccessor.ByteStride(weightView) ? (weightAccessor.ByteStride(weightView) / sizeof(float)) : tinygltf::GetTypeSizeInBytes(TINYGLTF_TYPE_VEC4);
//				}
//
//				hasSkin = (bufferJoints && bufferWeights);
//
//				for (size_t v = 0; v < posAccessor.count; v++)
//				{
//					Vertex vert{};
//					vert.pos = glm::vec4(glm::make_vec3(&bufferPos[v * posByteStride]), 1.0f);
//					vert.normal = glm::normalize(glm::vec3(bufferNormals ? glm::make_vec3(&bufferNormals[v * normByteStride]) : glm::vec3(0.0f)));
//					vert.uv0 = bufferTexCoordSet0 ? glm::make_vec2(&bufferTexCoordSet0[v * uv0ByteStride]) : glm::vec3(0.0f);
//					vert.uv1 = bufferTexCoordSet1 ? glm::make_vec2(&bufferTexCoordSet1[v * uv1ByteStride]) : glm::vec3(0.0f);
//
//					if (hasSkin)
//					{
//						switch (jointComponentType)
//						{
//						case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
//						{
//							const uint16_t* buf = static_cast<const uint16_t*>(bufferJoints);
//							vert.joint0 = glm::vec4(glm::make_vec4(&buf[v * jointByteStride]));
//							break;
//						}
//						case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
//						{
//							const uint8_t* buf = static_cast<const uint8_t*>(bufferJoints);
//							vert.joint0 = glm::vec4(glm::make_vec4(&buf[v * jointByteStride]));
//							break;
//						}
//						default:
//							// Not supported by spec
//							std::cerr << "Joint component type " << jointComponentType << " not supported!" << std::endl;
//							break;
//						}
//					}
//					else
//					{
//						vert.joint0 = glm::vec4(0.0f);
//					}
//					vert.weight0 = hasSkin ? glm::make_vec4(&bufferWeights[v * weightByteStride]) : glm::vec4(0.0f);
//					// Fix for all zero weights
//					if (glm::length(vert.weight0) == 0.0f)
//					{
//						vert.weight0 = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
//					}
//					vertexBuffer.push_back(vert);
//				}
//			}
//			// Indices
//			if (hasIndices)
//			{
//				const tinygltf::Accessor& accessor = model.accessors[primitive.indices > -1 ? primitive.indices : 0];
//				const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
//				const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];
//
//				indexCount = static_cast<uint32_t>(accessor.count);
//				const void* dataPtr = &(buffer.data[accessor.byteOffset + bufferView.byteOffset]);
//
//				switch (accessor.componentType)
//				{
//				case TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT:
//				{
//					const uint32_t* buf = static_cast<const uint32_t*>(dataPtr);
//					for (size_t index = 0; index < accessor.count; index++)
//					{
//						indexBuffer.push_back(buf[index] + vertexStart);
//					}
//					break;
//				}
//				case TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT:
//				{
//					const uint16_t* buf = static_cast<const uint16_t*>(dataPtr);
//					for (size_t index = 0; index < accessor.count; index++)
//					{
//						indexBuffer.push_back(buf[index] + vertexStart);
//					}
//					break;
//				}
//				case TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE:
//				{
//					const uint8_t* buf = static_cast<const uint8_t*>(dataPtr);
//					for (size_t index = 0; index < accessor.count; index++)
//					{
//						indexBuffer.push_back(buf[index] + vertexStart);
//					}
//					break;
//				}
//				default:
//					std::cerr << "Index component type " << accessor.componentType << " not supported!" << std::endl;
//					return;
//				}
//			}
//			Primitive* newPrimitive = new Primitive(indexStart, indexCount, vertexCount, primitive.material > -1 ? materials[primitive.material] : materials.back());
//			newPrimitive->SetBoundingBox(posMin, posMax);
//			newMesh->primitives.push_back(newPrimitive);
//		}
//		// Mesh BB from BBs of primitives
//		for (auto p : newMesh->primitives)
//		{
//			if (p->bb.valid && !newMesh->bb.valid)
//			{
//				newMesh->bb = p->bb;
//				newMesh->bb.valid = true;
//			}
//			newMesh->bb.min = glm::min(newMesh->bb.min, p->bb.min);
//			newMesh->bb.max = glm::max(newMesh->bb.max, p->bb.max);
//		}
//		newNode->mesh = newMesh;
//	}
//	if (parent)
//	{
//		parent->children.push_back(newNode);
//	}
//	else
//	{
//		nodes.push_back(newNode);
//	}
//	linearNodes.push_back(newNode);
//}
//
//void Model::LoadSkins(tinygltf::Model& gltfModel)
//{
//	for (tinygltf::Skin& source : gltfModel.skins)
//	{
//		Skin* newSkin = new Skin();
//		newSkin->name = source.name;
//
//		// Find skeleton root node
//		if (source.skeleton > -1)
//		{
//			newSkin->skeletonRoot = NodeFromIndex(source.skeleton);
//		}
//
//		// Find joint nodes
//		for (int jointIndex : source.joints)
//		{
//			Node* node = NodeFromIndex(jointIndex);
//			if (node)
//			{
//				newSkin->joints.push_back(NodeFromIndex(jointIndex));
//			}
//		}
//
//		// Get inverse bind matrices from buffer
//		if (source.inverseBindMatrices > -1)
//		{
//			const tinygltf::Accessor& accessor = gltfModel.accessors[source.inverseBindMatrices];
//			const tinygltf::BufferView& bufferView = gltfModel.bufferViews[accessor.bufferView];
//			const tinygltf::Buffer& buffer = gltfModel.buffers[bufferView.buffer];
//			newSkin->inverseBindMatrices.resize(accessor.count);
//			memcpy(newSkin->inverseBindMatrices.data(), &buffer.data[accessor.byteOffset + bufferView.byteOffset], accessor.count * sizeof(glm::mat4));
//		}
//
//		skins.push_back(newSkin);
//	}
//}
//
//void Model::LoadTextures(tinygltf::Model& gltfModel, VulkanDevice* device, VkQueue transferQueue)
//{
//	for (tinygltf::Texture& tex : gltfModel.textures)
//	{
//		tinygltf::Image image = gltfModel.images[tex.source];
//		TextureSampler textureSampler;
//		if (tex.sampler == -1)
//		{
//			// No sampler specified, use a default one
//			textureSampler.magFilter = VK_FILTER_LINEAR;
//			textureSampler.minFilter = VK_FILTER_LINEAR;
//			textureSampler.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
//			textureSampler.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
//			textureSampler.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
//		}
//		else
//		{
//			textureSampler = textureSamplers[tex.sampler];
//		}
//		Texture texture;
//		texture.FromglTfImage(image, textureSampler, device, transferQueue);
//		textures.push_back(texture);
//	}
//}
//
//VkSamplerAddressMode Model::GetVkWrapMode(int32_t wrapMode)
//{
//	switch (wrapMode)
//	{
//	case 10497:
//		return VK_SAMPLER_ADDRESS_MODE_REPEAT;
//	case 33071:
//		return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
//	case 33648:
//		return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
//	}
//}
//
//VkFilter Model::GetVkFilterMode(int32_t filterMode)
//{
//	switch (filterMode)
//	{
//	case 9728:
//		return VK_FILTER_NEAREST;
//	case 9729:
//		return VK_FILTER_LINEAR;
//	case 9984:
//		return VK_FILTER_NEAREST;
//	case 9985:
//		return VK_FILTER_NEAREST;
//	case 9986:
//		return VK_FILTER_LINEAR;
//	case 9987:
//		return VK_FILTER_LINEAR;
//	}
//}
//
//void Model::LoadTextureSamplers(tinygltf::Model& gltfModel)
//{
//	for (tinygltf::Sampler smpl : gltfModel.samplers)
//	{
//		TextureSampler sampler{};
//		sampler.minFilter = GetVkFilterMode(smpl.minFilter);
//		sampler.magFilter = GetVkFilterMode(smpl.magFilter);
//		sampler.addressModeU = GetVkWrapMode(smpl.wrapS);
//		sampler.addressModeV = GetVkWrapMode(smpl.wrapT);
//		sampler.addressModeW = sampler.addressModeV;
//		textureSamplers.push_back(sampler);
//	}
//}
//
//void Model::LoadMaterials(tinygltf::Model& gltfModel)
//{
//	for (tinygltf::Material& mat : gltfModel.materials)
//	{
//		Material material{};
//		if (mat.values.find("baseColorTexture") != mat.values.end())
//		{
//			material.baseColorTexture = &textures[mat.values["baseColorTexture"].TextureIndex()];
//			material.texCoordSets.baseColor = mat.values["baseColorTexture"].TextureTexCoord();
//		}
//		if (mat.values.find("metallicRoughnessTexture") != mat.values.end())
//		{
//			material.metallicRoughnessTexture = &textures[mat.values["metallicRoughnessTexture"].TextureIndex()];
//			material.texCoordSets.metallicRoughness = mat.values["metallicRoughnessTexture"].TextureTexCoord();
//		}
//		if (mat.values.find("roughnessFactor") != mat.values.end())
//		{
//			material.roughnessFactor = static_cast<float>(mat.values["roughnessFactor"].Factor());
//		}
//		if (mat.values.find("metallicFactor") != mat.values.end())
//		{
//			material.metallicFactor = static_cast<float>(mat.values["metallicFactor"].Factor());
//		}
//		if (mat.values.find("baseColorFactor") != mat.values.end())
//		{
//			material.baseColorFactor = glm::make_vec4(mat.values["baseColorFactor"].ColorFactor().data());
//		}
//		if (mat.additionalValues.find("normalTexture") != mat.additionalValues.end())
//		{
//			material.normalTexture = &textures[mat.additionalValues["normalTexture"].TextureIndex()];
//			material.texCoordSets.normal = mat.additionalValues["normalTexture"].TextureTexCoord();
//		}
//		if (mat.additionalValues.find("emissiveTexture") != mat.additionalValues.end())
//		{
//			material.emissiveTexture = &textures[mat.additionalValues["emissiveTexture"].TextureIndex()];
//			material.texCoordSets.emissive = mat.additionalValues["emissiveTexture"].TextureTexCoord();
//		}
//		if (mat.additionalValues.find("occlusionTexture") != mat.additionalValues.end())
//		{
//			material.occlusionTexture = &textures[mat.additionalValues["occlusionTexture"].TextureIndex()];
//			material.texCoordSets.occlusion = mat.additionalValues["occlusionTexture"].TextureTexCoord();
//		}
//		if (mat.additionalValues.find("alphaMode") != mat.additionalValues.end())
//		{
//			tinygltf::Parameter param = mat.additionalValues["alphaMode"];
//			if (param.string_value == "BLEND")
//			{
//				material.alphaMode = ALPHAMODE_BLEND;
//			}
//			if (param.string_value == "MASK")
//			{
//				material.alphaCutoff = 0.5f;
//				material.alphaMode = ALPHAMODE_MASK;
//			}
//		}
//		if (mat.additionalValues.find("alphaCutoff") != mat.additionalValues.end())
//		{
//			material.alphaCutoff = static_cast<float>(mat.additionalValues["alphaCutoff"].Factor());
//		}
//		if (mat.additionalValues.find("emissiveFactor") != mat.additionalValues.end())
//		{
//			material.emissiveFactor = glm::vec4(glm::make_vec3(mat.additionalValues["emissiveFactor"].ColorFactor().data()), 1.0);
//			material.emissiveFactor = glm::vec4(0.0f);
//		}
//
//		// Extensions
//		// @TODO: Find out if there is a nicer way of reading these properties with recent tinygltf headers
//		if (mat.extensions.find("KHR_materials_pbrSpecularGlossiness") != mat.extensions.end())
//		{
//			auto ext = mat.extensions.find("KHR_materials_pbrSpecularGlossiness");
//			if (ext->second.Has("specularGlossinessTexture"))
//			{
//				auto index = ext->second.Get("specularGlossinessTexture").Get("index");
//				material.extension.specularGlossinessTexture = &textures[index.Get<int>()];
//				auto texCoordSet = ext->second.Get("specularGlossinessTexture").Get("texCoord");
//				material.texCoordSets.specularGlossiness = texCoordSet.Get<int>();
//				material.pbrWorkflows.specularGlossiness = true;
//			}
//			if (ext->second.Has("diffuseTexture"))
//			{
//				auto index = ext->second.Get("diffuseTexture").Get("index");
//				material.extension.diffuseTexture = &textures[index.Get<int>()];
//			}
//			if (ext->second.Has("diffuseFactor"))
//			{
//				auto factor = ext->second.Get("diffuseFactor");
//				for (uint32_t i = 0; i < factor.ArrayLen(); i++)
//				{
//					auto val = factor.Get(i);
//					material.extension.diffuseFactor[i] = val.IsNumber() ? (float)val.Get<double>() : (float)val.Get<int>();
//				}
//			}
//			if (ext->second.Has("specularFactor"))
//			{
//				auto factor = ext->second.Get("specularFactor");
//				for (uint32_t i = 0; i < factor.ArrayLen(); i++)
//				{
//					auto val = factor.Get(i);
//					material.extension.specularFactor[i] = val.IsNumber() ? (float)val.Get<double>() : (float)val.Get<int>();
//				}
//			}
//		}
//
//		materials.push_back(material);
//	}
//	// Push a default material at the end of the list for meshes with no material assigned
//	materials.push_back(Material());
//}
//
//void Model::LoadAnimations(tinygltf::Model& gltfModel)
//{
//	for (tinygltf::Animation& anim : gltfModel.animations)
//	{
//		Animation animation{};
//		animation.name = anim.name;
//		if (anim.name.empty())
//		{
//			animation.name = std::to_string(animations.size());
//		}
//
//		// Samplers
//		for (auto& samp : anim.samplers)
//		{
//			AnimationSampler sampler{};
//
//			if (samp.interpolation == "LINEAR")
//			{
//				sampler.interpolation = InterpolationType::LINEAR;
//			}
//			if (samp.interpolation == "STEP")
//			{
//				sampler.interpolation = InterpolationType::STEP;
//			}
//			if (samp.interpolation == "CUBICSPLINE")
//			{
//				sampler.interpolation = InterpolationType::CUBICSPLINE;
//			}
//
//			// Read sampler input time values
//			{
//				const tinygltf::Accessor& accessor = gltfModel.accessors[samp.input];
//				const tinygltf::BufferView& bufferView = gltfModel.bufferViews[accessor.bufferView];
//				const tinygltf::Buffer& buffer = gltfModel.buffers[bufferView.buffer];
//
//				assert(accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
//
//				const void* dataPtr = &buffer.data[accessor.byteOffset + bufferView.byteOffset];
//				const float* buf = static_cast<const float*>(dataPtr);
//				for (size_t index = 0; index < accessor.count; index++)
//				{
//					sampler.inputs.push_back(buf[index]);
//				}
//
//				for (auto input : sampler.inputs)
//				{
//					if (input < animation.start)
//					{
//						animation.start = input;
//					}
//					if (input > animation.end)
//					{
//						animation.end = input;
//					}
//				}
//			}
//
//			// Read sampler output T/R/S values 
//			{
//				const tinygltf::Accessor& accessor = gltfModel.accessors[samp.output];
//				const tinygltf::BufferView& bufferView = gltfModel.bufferViews[accessor.bufferView];
//				const tinygltf::Buffer& buffer = gltfModel.buffers[bufferView.buffer];
//
//				assert(accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
//
//				const void* dataPtr = &buffer.data[accessor.byteOffset + bufferView.byteOffset];
//
//				switch (accessor.type)
//				{
//				case TINYGLTF_TYPE_VEC3:
//				{
//					const glm::vec3* buf = static_cast<const glm::vec3*>(dataPtr);
//					for (size_t index = 0; index < accessor.count; index++)
//					{
//						sampler.outputsVec4.push_back(glm::vec4(buf[index], 0.0f));
//					}
//					break;
//				}
//				case TINYGLTF_TYPE_VEC4:
//				{
//					const glm::vec4* buf = static_cast<const glm::vec4*>(dataPtr);
//					for (size_t index = 0; index < accessor.count; index++)
//					{
//						sampler.outputsVec4.push_back(buf[index]);
//					}
//					break;
//				}
//				default:
//				{
//					std::cout << "unknown type" << std::endl;
//					break;
//				}
//				}
//			}
//
//			animation.samplers.push_back(sampler);
//		}
//
//		// Channels
//		for (auto& source : anim.channels)
//		{
//			AnimationChannel channel{};
//
//			if (source.target_path == "rotation")
//			{
//				channel.path = PathType::ROTATION;
//			}
//			if (source.target_path == "translation")
//			{
//				channel.path = PathType::TRANSLATION;
//			}
//			if (source.target_path == "scale")
//			{
//				channel.path = PathType::SCALE;
//			}
//			if (source.target_path == "weights")
//			{
//				std::cout << "weights not yet supported, skipping channel" << std::endl;
//				continue;
//			}
//			channel.samplerIndex = source.sampler;
//			channel.node = NodeFromIndex(source.target_node);
//			if (!channel.node)
//			{
//				continue;
//			}
//
//			animation.channels.push_back(channel);
//		}
//
//		animations.push_back(animation);
//	}
//}
//
//void Model::LoadFromFile(std::string filename, VulkanDevice* device, VkQueue transferQueue, float scale)
//{
//	tinygltf::Model gltfModel;
//	tinygltf::TinyGLTF gltfContext;
//	std::string error;
//	std::string warning;
//
//	this->device = device;
//
//	bool binary = false;
//	size_t extpos = filename.rfind('.', filename.length());
//	if (extpos != std::string::npos)
//	{
//		binary = (filename.substr(extpos + 1, filename.length() - extpos) == "glb");
//	}
//
//	bool fileLoaded = binary ? gltfContext.LoadBinaryFromFile(&gltfModel, &error, &warning, filename.c_str()) : gltfContext.LoadASCIIFromFile(&gltfModel, &error, &warning, filename.c_str());
//
//	std::vector<uint32_t> indexBuffer;
//	std::vector<Vertex> vertexBuffer;
//
//	if (fileLoaded)
//	{
//		LoadTextureSamplers(gltfModel);
//		LoadTextures(gltfModel, device, transferQueue);
//		LoadMaterials(gltfModel);
//		// TODO: scene handling with no default scene
//		const tinygltf::Scene& scene = gltfModel.scenes[gltfModel.defaultScene > -1 ? gltfModel.defaultScene : 0];
//		for (size_t i = 0; i < scene.nodes.size(); i++)
//		{
//			const tinygltf::Node node = gltfModel.nodes[scene.nodes[i]];
//			LoadNode(nullptr, node, scene.nodes[i], gltfModel, indexBuffer, vertexBuffer, scale);
//		}
//		if (gltfModel.animations.size() > 0)
//		{
//			LoadAnimations(gltfModel);
//		}
//		LoadSkins(gltfModel);
//
//		for (auto node : linearNodes)
//		{
//			// Assign skins
//			if (node->skinIndex > -1)
//			{
//				node->skin = skins[node->skinIndex];
//			}
//			// Initial pose
//			if (node->mesh)
//			{
//				node->Update();
//			}
//		}
//	}
//	else
//	{
//		// TODO: throw
//		std::cerr << "Could not load gltf file: " << error << std::endl;
//		return;
//	}
//
//	extensions = gltfModel.extensionsUsed;
//
//	size_t vertexBufferSize = vertexBuffer.size() * sizeof(Vertex);
//	size_t indexBufferSize = indexBuffer.size() * sizeof(uint32_t);
//	indices.count = static_cast<uint32_t>(indexBuffer.size());
//
//	assert(vertexBufferSize > 0);
//
//	struct StagingBuffer
//	{
//		VkBuffer buffer;
//		VkDeviceMemory memory;
//	} vertexStaging, indexStaging;
//
//	// Create staging buffers
//	// Vertex data
//	VK_CHECK_RESULT(device->CreateBuffer(
//		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
//		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
//		vertexBufferSize,
//		&vertexStaging.buffer,
//		&vertexStaging.memory,
//		vertexBuffer.data()));
//	// Index data
//	if (indexBufferSize > 0)
//	{
//		VK_CHECK_RESULT(device->CreateBuffer(
//			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
//			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
//			indexBufferSize,
//			&indexStaging.buffer,
//			&indexStaging.memory,
//			indexBuffer.data()));
//	}
//
//	// Create device local buffers
//	// Vertex buffer
//	VK_CHECK_RESULT(device->CreateBuffer(
//		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
//		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
//		vertexBufferSize,
//		&vertices.buffer,
//		&vertices.memory));
//	// Index buffer
//	if (indexBufferSize > 0)
//	{
//		VK_CHECK_RESULT(device->CreateBuffer(
//			VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
//			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
//			indexBufferSize,
//			&indices.buffer,
//			&indices.memory));
//	}
//
//	// Copy from staging buffers
//	VkCommandBuffer copyCmd = device->CreateCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);
//
//	VkBufferCopy copyRegion = {};
//
//	copyRegion.size = vertexBufferSize;
//	vkCmdCopyBuffer(copyCmd, vertexStaging.buffer, vertices.buffer, 1, &copyRegion);
//
//	if (indexBufferSize > 0)
//	{
//		copyRegion.size = indexBufferSize;
//		vkCmdCopyBuffer(copyCmd, indexStaging.buffer, indices.buffer, 1, &copyRegion);
//	}
//
//	device->FlushCommandBuffer(copyCmd, transferQueue, true);
//
//	vkDestroyBuffer(device->logicalDevice, vertexStaging.buffer, nullptr);
//	vkFreeMemory(device->logicalDevice, vertexStaging.memory, nullptr);
//	if (indexBufferSize > 0)
//	{
//		vkDestroyBuffer(device->logicalDevice, indexStaging.buffer, nullptr);
//		vkFreeMemory(device->logicalDevice, indexStaging.memory, nullptr);
//	}
//
//	GetSceneDimensions();
//}
//
//void Model::DrawNode(Node* node, VkCommandBuffer commandBuffer)
//{
//	if (node->mesh)
//	{
//		for (Primitive* primitive : node->mesh->primitives)
//		{
//			vkCmdDrawIndexed(commandBuffer, primitive->indexCount, 1, primitive->firstIndex, 0, 0);
//		}
//	}
//	for (auto& child : node->children)
//	{
//		DrawNode(child, commandBuffer);
//	}
//}
//
//void Model::Draw(VkCommandBuffer commandBuffer)
//{
//	const VkDeviceSize offsets[1] = { 0 };
//	vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertices.buffer, offsets);
//	vkCmdBindIndexBuffer(commandBuffer, indices.buffer, 0, VK_INDEX_TYPE_UINT32);
//	for (auto& node : nodes)
//	{
//		DrawNode(node, commandBuffer);
//	}
//}
//
//void Model::CalculateBoundingBox(Node* node, Node* parent)
//{
//	BoundingBox parentBvh = parent ? parent->bvh : BoundingBox(dimensions.min, dimensions.max);
//
//	if (node->mesh)
//	{
//		if (node->mesh->bb.valid)
//		{
//			node->aabb = node->mesh->bb.GetAABB(node->GetMatrix());
//			if (node->children.size() == 0)
//			{
//				node->bvh.min = node->aabb.min;
//				node->bvh.max = node->aabb.max;
//				node->bvh.valid = true;
//			}
//		}
//	}
//
//	parentBvh.min = glm::min(parentBvh.min, node->bvh.min);
//	parentBvh.max = glm::min(parentBvh.max, node->bvh.max);
//
//	for (auto& child : node->children)
//	{
//		CalculateBoundingBox(child, node);
//	}
//}
//
//void Model::GetSceneDimensions()
//{
//	// Calculate binary volume hierarchy for all nodes in the scene
//	for (auto node : linearNodes)
//	{
//		CalculateBoundingBox(node, nullptr);
//	}
//
//	dimensions.min = glm::vec3(FLT_MAX);
//	dimensions.max = glm::vec3(-FLT_MAX);
//
//	for (auto node : linearNodes)
//	{
//		if (node->bvh.valid)
//		{
//			dimensions.min = glm::min(dimensions.min, node->bvh.min);
//			dimensions.max = glm::max(dimensions.max, node->bvh.max);
//		}
//	}
//
//	// Calculate scene aabb
//	aabb = glm::scale(glm::mat4(1.0f), glm::vec3(dimensions.max[0] - dimensions.min[0], dimensions.max[1] - dimensions.min[1], dimensions.max[2] - dimensions.min[2]));
//	aabb[3][0] = dimensions.min[0];
//	aabb[3][1] = dimensions.min[1];
//	aabb[3][2] = dimensions.min[2];
//}
//
//void Model::UpdateAnimation(uint32_t index, float time)
//{
//	if (animations.empty())
//	{
//		std::cout << ".glTF does not contain animation." << std::endl;
//		return;
//	}
//	if (index > static_cast<uint32_t>(animations.size()) - 1)
//	{
//		std::cout << "No animation with index " << index << std::endl;
//		return;
//	}
//	Animation& animation = animations[index];
//
//	bool updated = false;
//	for (auto& channel : animation.channels)
//	{
//		AnimationSampler& sampler = animation.samplers[channel.samplerIndex];
//		if (sampler.inputs.size() > sampler.outputsVec4.size())
//		{
//			continue;
//		}
//
//		for (size_t i = 0; i < sampler.inputs.size() - 1; i++)
//		{
//			if ((time >= sampler.inputs[i]) && (time <= sampler.inputs[i + 1]))
//			{
//				float u = std::max(0.0f, time - sampler.inputs[i]) / (sampler.inputs[i + 1] - sampler.inputs[i]);
//				if (u <= 1.0f)
//				{
//					switch (channel.path)
//					{
//					case PathType::TRANSLATION:
//					{
//						glm::vec4 trans = glm::mix(sampler.outputsVec4[i], sampler.outputsVec4[i + 1], u);
//						channel.node->translation = glm::vec3(trans);
//						break;
//					}
//					case PathType::SCALE:
//					{
//						glm::vec4 trans = glm::mix(sampler.outputsVec4[i], sampler.outputsVec4[i + 1], u);
//						channel.node->scale = glm::vec3(trans);
//						break;
//					}
//					case PathType::ROTATION:
//					{
//						glm::quat q1;
//						q1.x = sampler.outputsVec4[i].x;
//						q1.y = sampler.outputsVec4[i].y;
//						q1.z = sampler.outputsVec4[i].z;
//						q1.w = sampler.outputsVec4[i].w;
//						glm::quat q2;
//						q2.x = sampler.outputsVec4[i + 1].x;
//						q2.y = sampler.outputsVec4[i + 1].y;
//						q2.z = sampler.outputsVec4[i + 1].z;
//						q2.w = sampler.outputsVec4[i + 1].w;
//						channel.node->rotation = glm::normalize(glm::slerp(q1, q2, u));
//						break;
//					}
//					}
//					updated = true;
//				}
//			}
//		}
//	}
//	if (updated)
//	{
//		for (auto& node : nodes)
//		{
//			node->Update();
//		}
//	}
//}
//
//Node* Model::FindNode(Node* parent, uint32_t index)
//{
//	Node* nodeFound = nullptr;
//	if (parent->index == index)
//	{
//		return parent;
//	}
//
//	for (auto& child : parent->children)
//	{
//		nodeFound = FindNode(child, index);
//		if (nodeFound)
//		{
//			break;
//		}
//	}
//	return nodeFound;
//}
//
//Node* Model::NodeFromIndex(uint32_t index)
//{
//	Node* nodeFound = nullptr;
//	for (auto& node : nodes)
//	{
//		nodeFound = FindNode(node, index);
//		if (nodeFound)
//		{
//			break;
//		}
//	}
//	return nodeFound;
//}