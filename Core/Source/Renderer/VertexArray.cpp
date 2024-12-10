#include "VertexArray.h"

namespace Core {
	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_rendererId);
		glBindVertexArray(m_rendererId);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_rendererId);
	}

	void VertexArray::bind() const
	{
		glBindVertexArray(m_rendererId);
	}

	void VertexArray::unbind() const
	{
		glBindVertexArray(0);
	}

	void VertexArray::setVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		glBindVertexArray(m_rendererId);
		vertexBuffer->bind();
		unsigned int i = 0;
		size_t offset = 0;
		for (const auto& element : vertexBuffer->getLayout()) {
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(
				i++, 
				element.count, 
				element.type, 
				element.normalized, 
				vertexBuffer->getLayout().getStride(), 
				(const void*)offset
			);
			offset += element.count * BufferElement::sizeOfType(element.type);
		}
		m_vertexBuffer = vertexBuffer;
	}

	void VertexArray::setIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_rendererId);
		indexBuffer->bind();
		m_indexBuffer = indexBuffer;
	}

	VertexArray* VertexArray::create()
	{
		return new VertexArray();
	}

}