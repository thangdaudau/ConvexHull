#include "Core.h"
#include <glm/glm.hpp>

namespace Core {

	Application* CreateApplication() {
		return (Application*) new CoreApp();
	}

	CoreApp::CoreApp()
		: Application(),
		m_camera(new Camera<CameraType::thirdPerson>()),
		m_pointShader("Assets/Shader/point.glsl"),
		m_triangleShader("Assets/Shader/triangle.glsl"),
		m_lineShader("Assets/Shader/line.glsl")
	{
		m_vertexArray = createRef<VertexArray>();
		m_vertexArray->setIndexBuffer(createRef<IndexBuffer>(nullptr, 0));
		m_OxyzRenderer = createRef<OxyzRenderer>(m_camera);
		m_OxyzRenderer->setLineWidth(5);
		m_OxyzRenderer->setPointSize(7);

		eventSetup();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(glfwGetCurrentContext(), true);
		ImGui_ImplOpenGL3_Init();
	}

	CoreApp::~CoreApp()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void CoreApp::eventSetup()
	{
		getWindow()->setMouseButtonCallback(
			[this](int button, int action, int mods) 
			{
				if (button == GLFW_MOUSE_BUTTON_LEFT)
				{
					switch (action) 
					{
					case GLFW_PRESS: m_dragging = true, m_dragStart = true; break;
					case GLFW_RELEASE: m_dragging = false; break;
					}
				}
			}
		);
		getWindow()->setCursorPosCallback(
			[this](double xpos, double ypos) 
			{
				if (m_dragging) 
				{
					if (m_dragStart) 
						m_dragStart = false;
					else 
						m_camera->onDrag(float(xpos - m_lastDragX), float(ypos - m_lastDragY));
					m_lastDragX = xpos;
					m_lastDragY = ypos;
				}
			}
		);
		getWindow()->setScrollCallback(
			[this](double xoffset, double yoffset) 
			{
				m_camera->onScroll((float)xoffset, (float)yoffset);
			}
		);
		getWindow()->setKeyCallback(
			[this](int key, int scancode, int action, int mods) {
				if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
				{
					if (m_vRunning)
						m_paused ^= 1;
				}
				if (key == GLFW_KEY_UP && action == GLFW_REPEAT)
				{
					m_speed = std::min(m_speed + 0.1f, m_maxSpeed);
				}
				if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT)
				{
					m_speed = std::max(m_speed - 0.1f, m_minSpeed);
				}
				if (key == GLFW_KEY_UP && action == GLFW_PRESS)
				{
					m_speed = std::min(m_speed + 0.1f, m_maxSpeed);
				}
				if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
				{
					m_speed = std::max(m_speed - 0.1f, m_minSpeed);
				}
			}
		);
	}

	void CoreApp::onUpdate()
	{
		onRender();
		beginImGuiFrame();
		onImGuiFrame();
		endImGuiFrame();
	}

	void CoreApp::onRender()
	{
		double currentFrame = glfwGetTime();
		m_OxyzRenderer->setClearColor(glm::vec4(0.5f, 0.5f, 0.5f, 0.0f));
		m_OxyzRenderer->clear();
		m_OxyzRenderer->drawAxis();
		if (m_vRunning)
		{
			if (not m_paused)
			{
				m_deltaTime += currentFrame - m_lastFrame;
				// TODO
				if (m_type == ConvexHullAlgoType::GiftWrapping)
				{
					if (m_deltaTime > 1 / m_speed)
					{
						m_visualizer.nextState();
						m_deltaTime = 0.0;
					}
				}
				if (m_type == ConvexHullAlgoType::Incremental)
				{
					if (m_deltaTime > 4 / m_speed)
					{
						m_visualizer.nextState();
						m_deltaTime = 0.0;
					}
				}
			}
			auto drawLine = [&](const std::vector<unsigned int>& indices)
			{
				if (indices.size() == 0ull)
					return;
				m_lineShader.bind();
				m_lineShader.setUniformMat4f("u_ViewProjection", m_camera->getVP());
				m_lineShader.setUniform4f("u_FragColor", 0.1f, 0.6f, 0.6f, 0.95f);
				m_vertexArray->setIndexBuffer(createRef<IndexBuffer>(&indices[0], (int)indices.size()));
				m_OxyzRenderer->drawLines(m_vertexArray);
			};
			auto drawFace = [&](const std::vector<unsigned int>& indices, float v0, float v1, float v2, float v3)
			{
				if (indices.size() == 0ull)
					return;
				m_triangleShader.bind();
				m_triangleShader.setUniformMat4f("u_ViewProjection", m_camera->getVP());
				m_triangleShader.setUniform4f("u_FragColor", v0, v1, v2, v3);
				m_vertexArray->setIndexBuffer(createRef<IndexBuffer>(&indices[0], (int)indices.size()));
				m_OxyzRenderer->drawTriangles(m_vertexArray);
			};
			std::vector<unsigned int> edges;
			if (m_type == ConvexHullAlgoType::GiftWrapping)
			{
				m_pointShader.bind();
				m_pointShader.setUniformMat4f("u_ViewProjection", m_camera->getVP());
				m_OxyzRenderer->drawPoints(m_vertexArray, m_numberOfPoints);
				std::vector<unsigned int> indices;
				int count = m_visualizer.getCurrentIndex() + 1;
				for (const auto& [a, b, c] : m_visualizer.getFaces()) 
				{
					if (count-- == 0)
						break;
					indices.push_back(a);
					indices.push_back(b);
					indices.push_back(c);
					edges.push_back(a);
					edges.push_back(b);
					edges.push_back(b);
					edges.push_back(c);
					edges.push_back(c);
					edges.push_back(a);
				}
				drawLine(edges);
				drawFace(indices, 0.1f, 0.6f, 0.1f, 0.6f);
			}
			if (m_type == ConvexHullAlgoType::Incremental)
			{
				int count = m_visualizer.getRemainFaceCount();
				int pointCount = m_visualizer.getCurrentIndex();
				if (m_deltaTime > 1 / m_speed)
					m_visualizer.clearVisibleFace();
				if (m_deltaTime > 2 / m_speed)
					count = (int)m_visualizer.getFaces().size();
				if (m_deltaTime > 3 / m_speed)
					pointCount = std::min(m_numberOfPoints, pointCount + 1);
				
				m_vertexArray->bind();
				m_vertexArray->setIndexBuffer(createRef<IndexBuffer>(&m_visualizer.getPointOrder()[0], pointCount));
				m_pointShader.bind();
				m_pointShader.setUniformMat4f("u_ViewProjection", m_camera->getVP());
				m_OxyzRenderer->drawPoints(m_vertexArray);

				std::vector<unsigned int> indices;
				for (const auto& [a, b, c] : m_visualizer.getFaces()) 
				{
					if (count-- == 0)
						break;
					indices.push_back(a);
					indices.push_back(b);
					indices.push_back(c);
					edges.push_back(a);
					edges.push_back(b);
					edges.push_back(b);
					edges.push_back(c);
					edges.push_back(c);
					edges.push_back(a);
				}
				std::vector<unsigned int> visibleFace;
				for (const auto& [a, b, c] : m_visualizer.getVisibleFaces())
				{
					visibleFace.push_back(a);
					visibleFace.push_back(b);
					visibleFace.push_back(c);
					edges.push_back(a);
					edges.push_back(b);
					edges.push_back(b);
					edges.push_back(c);
					edges.push_back(c);
					edges.push_back(a);
				}
				drawLine(edges);
				drawFace(indices, 0.1f, 0.6f, 0.1f, 0.6f);
				drawFace(visibleFace, 0.6f, 0.1f, 0.1f, 0.6f);
			}
		}
		m_lastFrame = currentFrame;
	}

	void CoreApp::onImGuiFrame()
	{
		ImGui::Begin("Visualizer");

		if (m_vRunning)
			ImGui::BeginDisabled();
		if (ImGui::RadioButton("Gift Wrapping", (int)m_type == 0)) 
			m_type = (int)m_type != 0 ? ConvexHullAlgoType::GiftWrapping : ConvexHullAlgoType::none;
		if (ImGui::RadioButton("Incremental", (int)m_type == 1))
			m_type = (int)m_type != 1 ? ConvexHullAlgoType::Incremental : ConvexHullAlgoType::none;

		ImGui::Text("Number of points: ");
		ImGui::SameLine();
		ImGui::SliderInt("##point count", &m_numberOfPoints, 4, 500);
		if (m_vRunning)
			ImGui::EndDisabled();

		ImGui::Text("Speed: ");
		ImGui::SameLine();
		ImGui::SliderFloat("##speed", &m_speed, 0.01f, 50.0f);

		if (not m_vRunning) {
			if (ImGui::Button("Run"))
			{
				if ((int)m_type != -1)
				{
					m_visualizer.reset(m_type, m_numberOfPoints);
					std::vector<float> v;
					for (glm::vec3 p : m_visualizer.getPoints())
					{
						v.push_back(p.y);
						v.push_back(p.z);
						v.push_back(p.x);
					}
					m_vertexArray->setVertexBuffer(createRef<VertexBuffer>(
						&v[0],
						sizeof(float) * 3 * m_numberOfPoints,
						3                                         // position
					));
					m_vRunning = true;
				}
			}
		}
		else
		{
			if (m_paused && ImGui::Button("Continue"))
				m_paused = false;
			else if (not m_paused && ImGui::Button("Pause"))
				m_paused = true;
		}

		ImGui::Text("\n\n\n");
		if (ImGui::Button("Refresh"))
		{
			m_vRunning = false;
			m_paused = false;
		}

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	void CoreApp::beginImGuiFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void CoreApp::endImGuiFrame()
	{
		ImGuiIO& io = ImGui::GetIO();
		//io.DisplaySize = ImVec2(getWindow()->getWidth(), getWindow()->getWidth());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

}