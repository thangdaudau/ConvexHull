#pragma once
#include <vector>
#include "VertexArray.h"
#include "Buffer.h"
#include "Shader.h"
#include "Core/Base.h"

namespace Core {

	class Renderer
	{
	public:
		Renderer();
		void init();
		void setViewport(int x, int y, size_t width, size_t height);
		void setClearColor(const glm::vec4& color);
		void clear();

		void drawPoints(const Ref<VertexArray>& pointArray, int count = 0);
		void drawLines(const Ref<VertexArray>& lineArray, int count = 0);
		void drawTriangles(const Ref<VertexArray>& triangleArray, int count = 0);

		void setPointSize(float size);
		void setLineWidth(float width);
	};

}