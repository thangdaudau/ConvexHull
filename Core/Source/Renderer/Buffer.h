#pragma once
#include <glad/glad.h>
#include <vector>
#include <cassert>

namespace Core {

	struct BufferElement
	{
		GLint count;
		GLenum type;
		GLboolean normalized;

		static size_t sizeOfType(GLenum type) {
			assert(type == GL_FLOAT && "only support for GL_FLOAT");
			return sizeof(float);
		}
	};

	class BufferLayout
	{
	public:
		template<typename... T>
		BufferLayout(int count, T&&... args) 
			: m_stride()
		{
			pushFloat(count, std::forward<T>(args)...);
		}

		std::vector<BufferElement>::const_iterator begin() const { return m_BufferElements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_BufferElements.end(); }
		unsigned int getStride() const { return m_stride; }

	private:
		void pushFloat(int count);
		template<typename... T>
		void pushFloat(int head, T&&... rest) {
			pushFloat(head);
			pushFloat(std::forward<T>(rest)...);
		}

	private:
		std::vector<BufferElement> m_BufferElements;
		unsigned int m_stride;
	};


	class IndexBuffer
	{
	public:
		IndexBuffer(const unsigned int* data, int count);
		~IndexBuffer();

		void bind() const;
		void unbind() const;

		int getCount() const { return m_count; }

		static IndexBuffer* create(const unsigned int* data, int count);

	private:
		unsigned int m_rendererId;
		int m_count;
	};


	class VertexBuffer
	{
	public:
		// dynamic draw
		template<typename... T>
		explicit VertexBuffer(size_t size, T&&... layout_args)
			: m_layout(std::forward<T>(layout_args)...)
		{
			glGenBuffers(1, &m_rendererId);
			glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
			glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
		}
		// static draw
		template<typename... T>
		explicit VertexBuffer(const void* data, size_t size, T&&... layout_args)
			: m_layout(std::forward<T>(layout_args)...)
		{
			glGenBuffers(1, &m_rendererId);
			glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
			glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		}
		~VertexBuffer();

		void bind() const;
		void unbind() const;
		void setBuffer(const void* data, size_t size);

		const BufferLayout& getLayout() const { return m_layout; }
		void setLayout(const BufferLayout& layout) { m_layout = layout; }

		template<typename... T>
		static VertexBuffer* create(const void* data, size_t size, T&&... layout_args) { 
			return new VertexBuffer(data, size, std::forward<T>(layout_args)...); 
		}

	private:
		unsigned int m_rendererId;
		BufferLayout m_layout;
	};

}