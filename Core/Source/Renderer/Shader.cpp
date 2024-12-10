#include "Shader.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>


namespace Core {

#ifdef DEBUG
#define ENSURE_SHADER_BOUND assert(m_used && "Shader must be used before setting uniforms!")
#else 
#define ENSURE_SHADER_BOUND 0
#endif// DEBUG

	Shader::Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
	{
		m_rendererId = createProgram(vertexShaderSource, fragmentShaderSource);
	}

	Shader::Shader(const std::string& shaderPath) 
	{
		auto [vertexSource, fragmentSource] = getSource(shaderPath);
		m_rendererId = createProgram(vertexSource, fragmentSource);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_rendererId);
	}

	void Shader::bind() SHADER_CFUNC
	{
		glUseProgram(m_rendererId);
#ifdef DEBUG
		m_used = true;
#endif // DEBUG

	}

	void Shader::unbind() SHADER_CFUNC
	{
		glUseProgram(0);
#ifdef DEBUG
		m_used = false;
#endif // DEBUG
	}

	std::pair<std::string, std::string> Shader::getSource(const std::string& shaderPath) 
	{
		std::ifstream stream(shaderPath);
		enum class shaderType {
			NONE = -1, VERTEX = 0, FRAGMENT = 1
		};
		shaderType type = shaderType::NONE;

		std::string line;
		std::stringstream sstream[2];
		while (std::getline(stream, line)) {
			if (line.substr(0, 13) == "#vertexShader") 
			{
				type = shaderType::VERTEX;
			}
			else if (line.substr(0, 15) == "#fragmentShader") 
			{
				type = shaderType::FRAGMENT;
			}
			else if (type != shaderType::NONE) 
			{
				sstream[int(type)] << line << '\n';
			}
		}
		return std::pair(sstream[0].str(), sstream[1].str());
	}

	unsigned int Shader::compileShader(unsigned int type, const std::string& source) 
	{
		unsigned int shader = glCreateShader(type);
		const char* src = source.c_str();

		glShaderSource(shader, 1, &src, nullptr);
		glCompileShader(shader);

		int success;
		char infoLog[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (not success) 
		{
			glGetShaderInfoLog(shader, 512, nullptr, infoLog);
			std::cerr << "ERROR::SHADER::" << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << "::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		return shader;
	}

	unsigned int Shader::createProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) 
	{
		unsigned int shaderProgram = glCreateProgram();
		unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
		unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		int success;
		char infoLog[512];
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (not success) 
		{
			glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
			std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glValidateProgram(shaderProgram);
		return shaderProgram;
	}

	int Shader::getUniformLocation(const std::string& name) 
	{
		auto it = m_uniformLocationCache.find(name);
		if (it != m_uniformLocationCache.end()) 
		{
			return it->second;
		}
		int location = glGetUniformLocation(m_rendererId, name.c_str());
		assert(location != -1);
		return m_uniformLocationCache[name] = location;
	}

	void Shader::setUniform1i(const std::string& name, int v0) 
	{
		ENSURE_SHADER_BOUND;
		glUniform1i(getUniformLocation(name), v0);
	}

	void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3) 
	{
		ENSURE_SHADER_BOUND;
		glUniform4f(getUniformLocation(name), v0, v1, v2, v3);
	}

	void Shader::setUniformMat4f(const std::string& name, const glm::mat4& matrix)
	{
		ENSURE_SHADER_BOUND;
		glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
	}

}