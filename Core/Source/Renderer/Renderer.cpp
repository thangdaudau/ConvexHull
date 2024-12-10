#include "Renderer.h"

namespace Core {

	Renderer::Renderer()
	{
		init();
	}

	void Renderer::init()
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Renderer::setViewport(int x, int y, size_t width, size_t height)
	{
		glViewport(0, 0, width, height);
	}

	void Renderer::setClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void Renderer::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::drawPoints(const Ref<VertexArray>& pointArray, int count)
	{
		pointArray->bind();
		if (count == 0)
			glDrawElements(GL_POINTS, pointArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
		else
			glDrawArrays(GL_POINTS, 0, count);
	}

	void Renderer::drawLines(const Ref<VertexArray>& lineArray, int count)
	{
		lineArray->bind();
		if (count == 0)
			glDrawElements(GL_LINES, lineArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
		else
			glDrawArrays(GL_LINES, 0, count);
	}

	void Renderer::drawTriangles(const Ref<VertexArray>& triangleArray, int count)
	{
		triangleArray->bind();
		if (count == 0)
			glDrawElements(GL_TRIANGLES, triangleArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
		else
			glDrawArrays(GL_TRIANGLES, 0, count);
	}

	void Renderer::setPointSize(float size)
	{
		glPointSize(size);
	}

	void Renderer::setLineWidth(float width)
	{
		glLineWidth(width);
	}


}