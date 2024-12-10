#pragma once
#include "Application.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "Renderer/VertexArray.h"
#include "Renderer/Buffer.h"
#include "Renderer/Shader.h"
#include "Renderer/Camera.h"
#include "OxyzRenderer.h"
#include "Math/ConvexHullAlgos.h"
#include "Base.h"


namespace Core {

	class CoreApp : public Application
	{
	public:
		CoreApp();
		~CoreApp();
		void eventSetup();

		void onUpdate() override;
		void onRender();
		void onImGuiFrame();
		void beginImGuiFrame();
		void endImGuiFrame();
	private:
		double m_lastFrame = 0.0;
		double m_deltaTime = 0.0;
		float m_speed = 5.0f;
		const float m_minSpeed = 0.01f;
		const float m_maxSpeed = 50.0f;
		bool m_dragging = false;
		bool m_dragStart = false;
		double m_lastDragX = 0.0;
		double m_lastDragY = 0.0;
		Ref<VertexArray> m_vertexArray;
		Ref<OxyzRenderer> m_OxyzRenderer;
		Ref<Camera<CameraType::thirdPerson>> m_camera;
		Shader m_pointShader;
		Shader m_triangleShader;
		Shader m_lineShader;
		ConvexHullAlgos m_visualizer;
		ConvexHullAlgoType m_type = ConvexHullAlgoType::none;
		int m_numberOfPoints = 4;
		bool m_vRunning = false;
		bool m_paused = false;
	};

	Application* CreateApplication();
}