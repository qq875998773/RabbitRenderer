#include "ModelData/Node.h"

Node::~Node()
{
	if (mesh)
	{
		delete mesh;
	}
	for (auto& child : children)
	{
		delete child;
	}
}

glm::mat4 Node::LocalMatrix()
{
	return glm::translate(glm::mat4(1.0f), translation) * glm::mat4(rotation) * glm::scale(glm::mat4(1.0f), scale) * matrix;
}

glm::mat4 Node::GetMatrix()
{
	glm::mat4 m = LocalMatrix();
	Node* p = parent;
	while (p)
	{
		m = p->LocalMatrix() * m;
		p = p->parent;
	}
	return m;
}

void Node::Update()
{
	if (mesh)
	{
		glm::mat4 m = GetMatrix();
		if (skin)
		{
			mesh->uniformBlock.matrix = m;
			// Update join matrices
			glm::mat4 inverseTransform = glm::inverse(m);
			size_t numJoints = std::min((uint32_t)skin->joints.size(), MAX_NUM_JOINTS);
			for (size_t i = 0; i < numJoints; i++)
			{
				Node* jointNode = skin->joints[i];
				glm::mat4 jointMat = jointNode->GetMatrix() * skin->inverseBindMatrices[i];
				jointMat = inverseTransform * jointMat;
				mesh->uniformBlock.jointMatrix[i] = jointMat;
			}
			mesh->uniformBlock.jointcount = (float)numJoints;
			memcpy(mesh->uniformBuffer.mapped, &mesh->uniformBlock, sizeof(mesh->uniformBlock));
		}
		else
		{
			memcpy(mesh->uniformBuffer.mapped, &m, sizeof(glm::mat4));
		}
	}

	for (auto& child : children)
	{
		child->Update();
	}
}