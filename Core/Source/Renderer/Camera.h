#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Core {
	enum class CameraType {
		none, firstPerson, thirdPerson
	};

	template<CameraType>
	class Camera;


	template<>
	class Camera<CameraType::thirdPerson>
	{
	public:
		Camera(glm::vec3 pos = glm::vec3(8.0f, 8.0f, 8.0f), glm::vec3 center = glm::vec3());

		glm::mat4 getView() const;
		glm::mat4 getProjection() const;
		glm::mat4 getVP() const;

		void onDrag(float xoffset, float yoffset); // OpenGL does not have drag call back, just user defined
		void onScroll(float xoffset, float yoffset);

		const glm::vec3 getDirection() const { return m_cameraCenter - m_cameraPos; }
		const glm::vec3& getCenter() const { return m_cameraCenter; }
		const glm::vec3& getPosition() const { return m_cameraPos; }
		void setCenter(const glm::vec3& newCenter) { m_cameraCenter = newCenter; }
		void setPosition(const glm::vec3& newPosition) { m_cameraPos = newPosition; }
		void setZNear(float zNear) { m_zNear = zNear; }
		void setZFar(float zFar) { m_zFar = zFar; }
	private:
		static constexpr glm::vec3 s_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 m_cameraPos;
		glm::vec3 m_cameraCenter;

	private:
		float m_sensitivity = 0.25f;
		float m_fov = 45.0f;
		float m_aspect = 16.0f / 9.0f;
		float m_zNear = 0.1f;
		float m_zFar = 100.0f;
	};

}