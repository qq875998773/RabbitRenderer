#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>


/// @brief ���������
enum class CameraType
{
	lookat,			// Լ�����(����Ƶ�۲�)
	firstperson		// ��һ�˳����(�����Ϊ��۲�)
};

/// @brief �������
struct SMatrice
{
	glm::mat4 perspective	= glm::mat4(1.0f);	// ͸��
	glm::mat4 view			= glm::mat4(1.0f);
};

/// @brief �����̰���
struct SKey
{
	bool left = false;
	bool right = false;
	bool up = false;
	bool down = false;
};



/// @brief �������
class Camera
{
public:
	CameraType	type;			// ������� Ĭ��Լ�����
	glm::vec3	rotation;		// ��ת��
	glm::vec3	position;		// ��λ��
	float		rotationSpeed;	// ��ת�ٶ�
	float		movementSpeed;	// �ƶ��ٶ�
	bool		updated;		// �Ƿ�ˢ��
	SMatrice	matrices;		// �任���� 
	SKey		keys;			// �����̰���

public:
	 Camera();
	 ~Camera();

	/// @brief ��ȡ�Ƿ��ƶ�
	bool Moving();

	/// @brief ��ȡ���ü�����
	float GetNearClip();

	/// @brief ��ȡԶ�ü�����
	float GetFarClip();

	/// @brief ����͸��
	/// @param [in ] fov ͸��ͶӰ�Ķ���(һ��Ĭ��45��)
	/// @param [in ] aspect ��߱�
	/// @param [in ] znear ���ü���
	/// @param [in ] zfar Զ�ü���
	void SetPerspective(float fov, float aspect, float znear, float zfar);

	// ������ת
	void UpdateAspectRatio(float aspect);

	// ���ö�λ��
	void SetPosition(glm::vec3 position);

	// ������ת
	void SetRotation(glm::vec3 rotation);

	void Rotate(glm::vec3 delta);

	void SetTranslation(glm::vec3 translation);

	void Translate(glm::vec3 delta);

	/// @brief ���������
	/// @param [in ] deltaTime ����ʱ��
	void Update(float deltaTime);

	// ���������
	bool UpdatePad(glm::vec2 axisLeft, glm::vec2 axisRight, float deltaTime);

private:
	float m_fFov = 45.f;
	float m_fZnear = .1f;	// ���ü�����
	float m_fZfar = 256.f;	// Զ�ü�����

private:
	void UpdateViewMatrix();
};