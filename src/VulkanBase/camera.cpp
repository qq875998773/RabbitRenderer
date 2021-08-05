#include "camera.h"

Camera::Camera()
	: type(CameraType::lookat)
	, rotation(glm::vec3())
	, position(glm::vec3())
	, rotationSpeed(1.0f)
	, movementSpeed(1.0f)
	, updated(false)
{
}

Camera::~Camera()
{
}

bool Camera::Moving()
{
	return keys.left || keys.right || keys.up || keys.down;
}

float Camera::GetNearClip()
{
	return m_fZnear;
}

float Camera::GetFarClip()
{
	return m_fZfar;
}

void Camera::SetPerspective(float fov, float aspect, float znear, float zfar)
{
	m_fFov = fov;
	m_fZnear = znear;
	m_fZfar = zfar;
	matrices.perspective = glm::perspective(glm::radians(fov), aspect, znear, zfar);
}

void Camera::UpdateAspectRatio(float aspect)
{
	matrices.perspective = glm::perspective(glm::radians(m_fFov), aspect, m_fZnear, m_fZfar);
}

void Camera::SetPosition(glm::vec3 position)
{
	this->position = position;
	UpdateViewMatrix();
}

void Camera::SetRotation(glm::vec3 rotation)
{
	this->rotation = rotation;
	UpdateViewMatrix();
}

void Camera::Rotate(glm::vec3 delta)
{
	this->rotation += delta;
	UpdateViewMatrix();
}

void Camera::SetTranslation(glm::vec3 translation)
{
	this->position = translation;
	UpdateViewMatrix();
}

void Camera::Translate(glm::vec3 delta)
{
	this->position += delta;
	UpdateViewMatrix();
}

void Camera::Update(float deltaTime)
{
	updated = false;
	if (type == CameraType::firstperson)
	{
		if (Moving())
		{
			glm::vec3 camFront;
			camFront.x = -cos(glm::radians(rotation.x)) * sin(glm::radians(rotation.y));
			camFront.y = sin(glm::radians(rotation.x));
			camFront.z = cos(glm::radians(rotation.x)) * cos(glm::radians(rotation.y));
			camFront = glm::normalize(camFront);

			float moveSpeed = deltaTime * movementSpeed;

			if (keys.up)
			{
				position += camFront * moveSpeed;
			}
			if (keys.down)
			{
				position -= camFront * moveSpeed;
			}
			if (keys.left)
			{
				position -= glm::normalize(glm::cross(camFront, glm::vec3(0.0f, 1.0f, 0.0f))) * moveSpeed;
			}
			if (keys.right)
			{
				position += glm::normalize(glm::cross(camFront, glm::vec3(0.0f, 1.0f, 0.0f))) * moveSpeed;
			}

			UpdateViewMatrix();
		}
	}
}

bool Camera::UpdatePad(glm::vec2 axisLeft, glm::vec2 axisRight, float deltaTime)
{
	bool retVal = false;

	if (type == CameraType::firstperson)
	{
		// Use the common console thumbstick layout		
		// Left = view, right = move

		const float deadZone = 0.0015f;
		const float range = 1.0f - deadZone;

		glm::vec3 camFront;
		camFront.x = -cos(glm::radians(rotation.x)) * sin(glm::radians(rotation.y));
		camFront.y = sin(glm::radians(rotation.x));
		camFront.z = cos(glm::radians(rotation.x)) * cos(glm::radians(rotation.y));
		camFront = glm::normalize(camFront);

		float moveSpeed = deltaTime * movementSpeed * 2.0f;
		float rotSpeed = deltaTime * rotationSpeed * 50.0f;

		// 移动
		if (fabsf(axisLeft.y) > deadZone)
		{
			float pos = (fabsf(axisLeft.y) - deadZone) / range;
			position -= camFront * pos * ((axisLeft.y < 0.0f) ? -1.0f : 1.0f) * moveSpeed;
			retVal = true;
		}
		if (fabsf(axisLeft.x) > deadZone)
		{
			float pos = (fabsf(axisLeft.x) - deadZone) / range;
			position += glm::normalize(glm::cross(camFront, glm::vec3(0.0f, 1.0f, 0.0f))) * pos * ((axisLeft.x < 0.0f) ? -1.0f : 1.0f) * moveSpeed;
			retVal = true;
		}

		// 旋转
		if (fabsf(axisRight.x) > deadZone)
		{
			float pos = (fabsf(axisRight.x) - deadZone) / range;
			rotation.y += pos * ((axisRight.x < 0.0f) ? -1.0f : 1.0f) * rotSpeed;
			retVal = true;
		}
		if (fabsf(axisRight.y) > deadZone)
		{
			float pos = (fabsf(axisRight.y) - deadZone) / range;
			rotation.x -= pos * ((axisRight.y < 0.0f) ? -1.0f : 1.0f) * rotSpeed;
			retVal = true;
		}
	}
	else
	{
		// todo: move code from example base class for look-at
	}

	if (retVal)
	{
		UpdateViewMatrix();
	}

	return retVal;
}

void Camera::UpdateViewMatrix()
{
	glm::mat4 rotM = glm::mat4(1.0f);	// 旋转矩阵
	glm::mat4 transM;					// 移动矩阵

	// 设置相机在xyz坐标轴的旋转矩阵
	rotM = glm::rotate(rotM, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	rotM = glm::rotate(rotM, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	rotM = glm::rotate(rotM, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	// 设置移动矩阵  /glm::mat4(1.0f)初始化一个单位矩阵
	transM = glm::translate(glm::mat4(1.0f), position * glm::vec3(1.0f, 1.0f, -1.0f));

	if (type == CameraType::firstperson)
	{
		matrices.view = rotM * transM; // 第一人称 先旋转再移动
	}
	else
	{
		matrices.view = transM * rotM; // 约束相机 先移动再旋转
	}

	updated = true;
}