#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>


/// @brief 摄像机类型
enum class CameraType
{
	lookat,			// 约束相机(相机绕点观察)
	firstperson		// 第一人称相机(以相机为点观察)
};

/// @brief 相机矩阵
struct SMatrice
{
	glm::mat4 perspective	= glm::mat4(1.0f);	// 透视
	glm::mat4 view			= glm::mat4(1.0f);
};

/// @brief 鼠标键盘按键
struct SKey
{
	bool left = false;
	bool right = false;
	bool up = false;
	bool down = false;
};



/// @brief 摄像机类
class Camera
{
public:
	CameraType	type;			// 设置相机 默认约束相机
	glm::vec3	rotation;		// 旋转轴
	glm::vec3	position;		// 定位点
	float		rotationSpeed;	// 旋转速度
	float		movementSpeed;	// 移动速度
	bool		updated;		// 是否刷新
	SMatrice	matrices;		// 变换矩阵 
	SKey		keys;			// 鼠标键盘按键

public:
	 Camera();
	 ~Camera();

	/// @brief 获取是否移动
	bool Moving();

	/// @brief 获取近裁剪距离
	float GetNearClip();

	/// @brief 获取远裁剪距离
	float GetFarClip();

	/// @brief 设置透视
	/// @param [in ] fov 透视投影的度数(一般默认45度)
	/// @param [in ] aspect 宽高比
	/// @param [in ] znear 近裁剪面
	/// @param [in ] zfar 远裁剪面
	void SetPerspective(float fov, float aspect, float znear, float zfar);

	// 更新旋转
	void UpdateAspectRatio(float aspect);

	// 设置定位点
	void SetPosition(glm::vec3 position);

	// 设置旋转
	void SetRotation(glm::vec3 rotation);

	void Rotate(glm::vec3 delta);

	void SetTranslation(glm::vec3 translation);

	void Translate(glm::vec3 delta);

	/// @brief 更新摄像机
	/// @param [in ] deltaTime 增量时间
	void Update(float deltaTime);

	// 更新摄像机
	bool UpdatePad(glm::vec2 axisLeft, glm::vec2 axisRight, float deltaTime);

private:
	float m_fFov = 45.f;
	float m_fZnear = .1f;	// 近裁剪距离
	float m_fZfar = 256.f;	// 远裁剪距离

private:
	void UpdateViewMatrix();
};