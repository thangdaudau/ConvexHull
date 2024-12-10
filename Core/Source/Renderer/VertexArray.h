#pragma once
#include <glad/glad.h>
#include "Buffer.h"
#include "Core/Base.h"

namespace Core {
	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		void bind() const;
		void unbind() const;

		void setVertexBuffer(const Ref<VertexBuffer>& vertexBuffer);
		void setIndexBuffer(const Ref<IndexBuffer>& indexBuffer);
		Ref<VertexBuffer> getVertexBuffer() const { return m_vertexBuffer; }
		Ref<IndexBuffer> getIndexBuffer() const { return m_indexBuffer; }

		static VertexArray* create();

	private:
		unsigned int m_rendererId;
		Ref<VertexBuffer> m_vertexBuffer;
		Ref<IndexBuffer> m_indexBuffer;
	};

}
