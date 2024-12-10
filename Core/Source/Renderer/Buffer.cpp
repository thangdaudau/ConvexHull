#include "Buffer.h"

namespace Core {

	void BufferLayout::pushFloat(int count)
	{
		m_BufferElements.emplace_back(count, GL_FLOAT, GL_FALSE);
		m_stride += count * sizeof(float);
	}


	IndexBuffer::IndexBuffer(const unsigned int* data, int count) : m_count(count) {
		glGenBuffers(1, &m_rendererId);
		// From TheCherno
		// GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
		// Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state. 
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_rendererId);
	}

	void IndexBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId);
	}

	void IndexBuffer::unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	IndexBuffer* IndexBuffer::create(const unsigned int* data, int count)
	{
		return new IndexBuffer(data, count);
	}



	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_rendererId);
	}

	void VertexBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
	}

	void VertexBuffer::unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::setBuffer(const void* data, size_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
		// may cause GL_INVALID_VALUE (501) if size > current buffer size
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

}