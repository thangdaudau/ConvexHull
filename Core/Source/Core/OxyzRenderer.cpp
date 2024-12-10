#include "OxyzRenderer.h"

namespace Core {

    OxyzRenderer::OxyzRenderer(const Ref<Camera<CameraType::thirdPerson>>& camera)
        : m_camera(camera)
    {
        init();
    }

    void OxyzRenderer::init()
    {
        m_axis.reset(VertexArray::create());
        auto axisBuf = createRef<VertexBuffer>(
            sizeof(float) * 7 * (3 + 3),    // 6 vertices(3 lines)
            3,                              // aPosition
            4                               // aColor
        );
        m_axis->setVertexBuffer(axisBuf);

        const char* vAxisShader = R"(
			#version 420 core
			layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec4 aColor;

			uniform mat4 ViewProjection;
            
            out vec4 axisColor;

			void main()
			{
				gl_Position = ViewProjection * vec4(aPos, 1.0f);
                axisColor = aColor;
			}
		)";
        const char* fAxisShader = R"(
			#version 420 core
            in vec4 axisColor;
			out vec4 FragColor;
			void main()
			{
				FragColor = axisColor;
			}
		)";
        m_axisShader = createRef<Shader>(vAxisShader, fAxisShader);
    }

    void OxyzRenderer::drawAxis()
    {
        float axisLen = glm::length(m_camera->getDirection()) * 0.5f;
        float vertices[42] = {
                // aPosition                    // aColor
            -axisLen, 0.0f,    0.0f,         0.0f, 1.0f, 0.0f, 1.0f,
            axisLen,  0.0f,    0.0f,         0.0f, 1.0f, 0.0f, 1.0f,
            0.0f,    -axisLen, 0.0f,         0.0f, 0.0f, 1.0f, 1.0f,
            0.0f,    axisLen,  0.0f,         0.0f, 0.0f, 1.0f, 1.0f,
            0.0f,    0.0f,    -axisLen,      1.0f, 0.0f, 0.0f, 1.0f,
            0.0f,    0.0f,    axisLen,       1.0f, 0.0f, 0.0f, 1.0f,
        };
        m_axis->getVertexBuffer()->setBuffer(vertices, 42 * sizeof(float));
        m_axisShader->bind();
        m_axisShader->setUniformMat4f("ViewProjection", m_camera->getVP());
        drawLines(m_axis, 36);

        // TODO: more
    }

}