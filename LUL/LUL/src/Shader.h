#pragma once
#include <string>
#include <glm/matrix.hpp>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

enum class ShaderType
{
	NONE = -1, VERTEX = 0, FRAGMENT = 1
};

class Shader
{
private:
	int m_RendererID;
	std::string m_Filepath;
public:
	Shader(const std::string& path);
	Shader(const std::string& vertexPath, const std::string& fragmentPath, 
		const std::string& geometryPath = "");
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform1i(const std::string& name, int value) const;
	void SetUniform1f(const std::string& name, float value) const;
	void SetUniform2f(const std::string& name, const glm::vec2& vec) const;
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) const;
	void SetUniform4f(const std::string& name, const glm::vec4& vec) const;
	void SetUniform3f(const std::string& name, const glm::vec3& vec) const;
	void SetUniformMatrix4f(const std::string& name, const glm::mat4& matrix) const;
private:
	ShaderProgramSource ParseShader(const std::string& filepath);
	std::string ReadShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader, 
		const std::string& geometryShader = "");
	int GetUniformLocation(const std::string& uniformname) const;
};