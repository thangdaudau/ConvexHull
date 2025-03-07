#pragma once
#include "Renderer/Renderer.h"
#include "Renderer/Camera.h"
#include "Core/Base.h"
#include "Renderer/Shader.h"

namespace Core {

	class OxyzRenderer : public Renderer
	{
	public:
		OxyzRenderer(const Ref<Camera<CameraType::thirdPerson>>& camera);
		void init();
		void drawAxis();


	private:
		Ref<VertexArray> m_axis;
		Ref<VertexArray> m_arrow;
		Ref<Shader> m_axisShader;
		Ref<Shader> m_arrowShader;
		Ref<Camera<CameraType::thirdPerson>> m_camera;
	};

}