#include "Camera.h"

namespace Core {
	Camera<CameraType::thirdPerson>::Camera(glm::vec3 pos, glm::vec3 center)
		: m_cameraPos(pos), m_cameraCenter(center)
	{
	}

	glm::mat4 Camera<CameraType::thirdPerson>::getView() const
	{
		return glm::lookAt(m_cameraPos, m_cameraCenter, s_cameraUp);
	}

	glm::mat4 Camera<CameraType::thirdPerson>::getProjection() const
	{
		return glm::perspective(glm::radians(m_fov), m_aspect, m_zNear, m_zFar);
	}

	glm::mat4 Camera<CameraType::thirdPerson>::getVP() const
	{
		return getProjection() * getView();
	}

	void Camera<CameraType::thirdPerson>::onDrag(float xoffset, float yoffset)
	{
		glm::vec3 dir = m_cameraCenter - m_cameraPos;
		float radius = glm::length(dir);
		float phi = std::atan2(-dir.x, -dir.z) - glm::radians(xoffset* m_sensitivity);
		float theta = std::acosf(-dir.y / radius) - glm::radians(yoffset * m_sensitivity);
		if (theta < glm::radians(1.0f)) {
			theta = glm::radians(1.0f);
		}
		if (theta > glm::radians(179.0f)) {
			theta = glm::radians(179.0f);
		}
		glm::vec3 d;
		d.z = glm::sin(theta) * glm::cos(phi);
		d.x = glm::sin(theta) * glm::sin(phi);
		d.y = glm::cos(theta);
		m_cameraPos = m_cameraCenter + radius * d;
	}

	void Camera<CameraType::thirdPerson>::onScroll(float xoffset, float yoffset)
	{
		m_cameraPos -= 0.25f * (m_cameraPos - m_cameraCenter) * yoffset;
		setZFar(2.5f * glm::length(m_cameraPos - m_cameraCenter));
	}
}