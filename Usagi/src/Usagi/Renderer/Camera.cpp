#include "usgpch.h"
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Usagi/Input.h"
#include "Usagi/Keycodes.h"

#include <glm/gtc/quaternion.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>


#define M_PI 3.1415926

namespace Usagi {
	//========================ORTHOGRAPHIC CAMERA======================================================
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top) 
		:m_ProjectionMatrix(glm::ortho(left, right, bottom, top, - 1.0f, 1.0f)),m_ViewMatrix(1.0f)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	};

	void OrthographicCamera::RecalculateViewMatrix() {
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * 
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));
		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}


	//========================PROJECTION CAMERA======================================================
	ProjectionCamera::ProjectionCamera(const glm::mat4& projectionMatrix)
		: m_ProjectionMatrix(projectionMatrix)
	{
		// Sensible defaults
		m_PanSpeed = 0.0015f;
		m_RotationSpeed = 0.002f;
		m_ZoomSpeed = 0.2f;

		m_Position = { -100, 100, 100 };
		m_Rotation = glm::vec3(90.0f, 0.0f, 0.0f);

		m_FocalPoint = glm::vec3(0.0f);
		m_Distance = glm::distance(m_Position, m_FocalPoint);

		m_Yaw = 3.0f * (float)M_PI / 4.0f;
		m_Pitch = M_PI / 4.0f;
	}

	void ProjectionCamera::Focus()
	{
	}

	void ProjectionCamera::Update()
	{
		if (Usagi::Input::IsKeyPressed(USG_KEY_LEFT_ALT))
		{
			const glm::vec2& mouse{ Usagi::Input::GetMouseX(), Usagi::Input::GetMouseY() };
			glm::vec2 delta = mouse - m_InitialMousePosition;
			m_InitialMousePosition = mouse;

			if (Usagi::Input::IsMouseButtonPressed(USG_MOUSE_BUTTON_MIDDLE))
				MousePan(delta);
			else if (Usagi::Input::IsMouseButtonPressed(USG_MOUSE_BUTTON_LEFT))
				MouseRotate(delta);
			else if (Usagi::Input::IsMouseButtonPressed(USG_MOUSE_BUTTON_RIGHT))
				MouseZoom(delta.y);
		}

		m_Position = CalculatePosition();

		glm::quat orientation = GetOrientation();
		m_Rotation = glm::eulerAngles(orientation) * (180.0f / (float)M_PI);
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 1)) * glm::toMat4(glm::conjugate(orientation)) * glm::translate(glm::mat4(1.0f), -m_Position);
	}

	void ProjectionCamera::MousePan(const glm::vec2& delta)
	{
		m_FocalPoint += -GetRightDirection() * delta.x * m_PanSpeed * m_Distance;
		m_FocalPoint += GetUpDirection() * delta.y * m_PanSpeed * m_Distance;
	}

	void ProjectionCamera::MouseRotate(const glm::vec2& delta)
	{
		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		m_Yaw += yawSign * delta.x * m_RotationSpeed;
		m_Pitch += delta.y * m_RotationSpeed;
	}

	void ProjectionCamera::MouseZoom(float delta)
	{
		m_Distance -= delta * m_ZoomSpeed;
		if (m_Distance < 1.0f)
		{
			m_FocalPoint += GetForwardDirection();
			m_Distance = 1.0f;
		}
	}

	glm::vec3 ProjectionCamera::GetUpDirection()
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 ProjectionCamera::GetRightDirection()
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 ProjectionCamera::GetForwardDirection()
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::vec3 ProjectionCamera::CalculatePosition()
	{
		return m_FocalPoint - GetForwardDirection() * m_Distance;
	}

	glm::quat ProjectionCamera::GetOrientation()
	{
		return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
	}
}

