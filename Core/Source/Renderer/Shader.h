#pragma once
#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <unordered_map>

namespace Core {
#ifndef DEBUG
#define SHADER_CFUNC const
#else
#define SHADER_CFUNC
#endif // DEBUG


	class Shader
	{
	public:
		Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
		Shader(const std::string& path);
		~Shader();

		static std::pair<std::string, std::string> getSource(const std::string& shaderPath);
		static unsigned int compileShader(unsigned int type, const std::string& source);
		static unsigned int createProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);

		void bind() SHADER_CFUNC;
		void unbind() SHADER_CFUNC;

	private:
		int getUniformLocation(const std::string& name);

	public:
		void setUniform1i(const std::string& name, int v0);
		void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
		void setUniformMat4f(const std::string& name, const glm::mat4& matrix);


	private:
		unsigned int m_rendererId;
		std::unordered_map<std::string, int> m_uniformLocationCache;
#ifdef DEBUG
		bool m_used = false;
#endif // !DEBUG
	};

}