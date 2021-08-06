#include "ModelData/Primitive.h"

Primitive::Primitive(uint32_t firstIndex, uint32_t indexCount, uint32_t vertexCount, Material& material) 
	: firstIndex(firstIndex), indexCount(indexCount), vertexCount(vertexCount), material(material) 
{
	hasIndices = indexCount > 0;
}

void Primitive::SetBoundingBox(glm::vec3 min, glm::vec3 max)
{
	bb.min = min;
	bb.max = max;
	bb.valid = true;
}