#pragma once
#include <string>
#include <glm/matrix.hpp>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	int m_RendererID;
	std::string m_Filepath;
public:
	Shader(const std::string& path);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform1i(const std::string& name, int value) const;
	void SetUniform1f(const std::string& name, float value) const;
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) const;
	void SetUniform4f(const std::string& name, const glm::vec4& vec) const;
	void SetUniform3f(const std::string& name, const glm::vec3& vec) const;
	void SetUniformMatrix4f(const std::string& name, const glm::mat4& matrix) const;
private:
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	int GetUniformLocation(const std::string& uniformname) const;
};