#pragma once
#include <glew/glew.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <string>

class ShaderProgram
{
public:
	ShaderProgram(const GLchar* vertexPath, const GLchar* fragmentPath);
	~ShaderProgram();

	// shader를 활성화하고 사용합니다.
	void use();
	// Uniform 유틸리티 함수들
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec2(const std::string& name, const glm::vec2& value) const;
	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setMat4(const std::string& name, glm::mat4 value, bool transpose = false) const;

private:
	GLuint ref;

	std::string ShaderCode(const GLchar* path) const;

	class Shader 
	{
	public:
		Shader(GLenum shaderType, const char* shaderCode);
		~Shader();

		GLuint Ref() const;
	private:
		GLuint ref;
	};
};
