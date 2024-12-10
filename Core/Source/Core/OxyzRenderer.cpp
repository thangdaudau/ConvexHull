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

        unsigned int indices[15] = {
            0, 1, 4,
            1, 2, 4,
            2, 3, 4,
            0, 1, 2,
            2, 3, 0
        };
        m_arrow.reset(VertexArray::create());
        m_arrow->setIndexBuffer(createRef<IndexBuffer>(
            indices,
            int(sizeof(indices) / sizeof(float))
        ));
        m_arrow->setVertexBuffer(createRef<VertexBuffer>(
            sizeof(float) * 5 * 3,
            3
        ));
        m_arrowShader = createRef<Shader>("Assets/Shader/triangle.glsl");
    }

    void OxyzRenderer::drawAxis()
    {
        float axisLen = glm::length(m_camera->getDirection()) * 0.5f;
        {
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
        }
        {
            m_arrowShader->bind();
            float a = axisLen / 100;
            float verticies[15] = {
                -a,     0.0f,       -a,
                -a,     0.0f,       a,
                a,      0.0f,       a,
                a,      0.0f,       -a,
                0.0f,   3.0f * a,  0.0f,
            };
            m_arrow->getVertexBuffer()->setBuffer(verticies, sizeof(verticies));

            glm::mat4 model;
            model = glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(axisLen, 0.0f, 0.0f)), 
                                glm::radians(-90.0f), 
                                glm::vec3(0.0f, 0.0f, 1.0f));
            m_arrowShader->setUniform4f("u_FragColor", 0.0f, 1.0f, 0.0f, 1.0f);
            m_arrowShader->setUniformMat4f("u_ViewProjection", m_camera->getVP() * model);
            drawTriangles(m_arrow);

            model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, axisLen, 0.0f));
            m_arrowShader->setUniform4f("u_FragColor", 0.0f, 0.0f, 1.0f, 1.0f);
            m_arrowShader->setUniformMat4f("u_ViewProjection", m_camera->getVP() * model);
            drawTriangles(m_arrow);

            model = glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, axisLen)),
                                glm::radians(90.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f));
            m_arrowShader->setUniform4f("u_FragColor", 1.0f, 0.0f, 0.0f, 1.0f);
            m_arrowShader->setUniformMat4f("u_ViewProjection", m_camera->getVP() * model);
            drawTriangles(m_arrow);
        }
    }

}