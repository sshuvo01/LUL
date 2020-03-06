#include "Shader.h"
#include "Renderer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

Shader::Shader(const std::string & path)
	: m_Filepath(path), m_RendererID(0)
{
	ShaderProgramSource shaders = ParseShader(m_Filepath);
	m_RendererID = CreateShader(shaders.VertexSource, shaders.FragmentSource);
	Bind();
}

Shader::~Shader()
{
	//GLCALL(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
	GLCALL(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GLCALL(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string & name, int value) const
{
	Bind();
	int loc = GetUniformLocation(name);
	if (loc == -1) return;

	GLCALL(glUniform1i(loc, value));
}

void Shader::SetUniform1f(const std::string & name, float value) const
{
	Bind();
	int loc = GetUniformLocation(name);
	if (loc == -1) return;

	GLCALL(glUniform1f(loc, value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) const
{
	Bind();
	int loc = GetUniformLocation(name);
	if (loc == -1) return;

	GLCALL(glUniform4f(loc, v0, v1, v2, v3));
}

void Shader::SetUniform4f(const std::string & name, const glm::vec4 & vec) const
{
	Bind();
	int loc = GetUniformLocation(name);
	if (loc == -1) return;

	GLCALL(glUniform4f(loc, vec[0], vec[1], vec[2], vec[3]));
}

void Shader::SetUniform3f(const std::string & name, const glm::vec3 & vec) const
{
	Bind();
	int loc = GetUniformLocation(name);
	if (loc == -1) return;

	GLCALL(glUniform3f(loc, vec[0], vec[1], vec[2]));
}

void Shader::SetUniformMatrix4f(const std::string & name, const glm::mat4 & matrix) const
{
	Bind();
	int loc = GetUniformLocation(name);
	if (loc == -1) return;

	GLCALL(glUniformMatrix4fv(loc, 1, GL_FALSE, &matrix[0][0]));
}

ShaderProgramSource Shader::ParseShader(const std::string & filepath)
{
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::ifstream stream(filepath);

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	GLCALL(glShaderSource(id, 1, &src, nullptr));
	GLCALL(glCompileShader(id));

	int result;
	GLCALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		GLCALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));

		GLCALL(glGetShaderInfoLog(id, length, &length, message));

		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "VERTEX " : "FRAGMENT ");
		std::cout << message << std::endl;
		GLCALL(glDeleteShader(id));
		return 0;
	}

	return id;
}

unsigned int Shader::CreateShader(const std::string & vertexShader, const std::string & fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCALL( glAttachShader(program, vs) );
	GLCALL( glAttachShader(program, fs) );
	GLCALL( glLinkProgram(program));
	GLCALL( glValidateProgram(program));

	GLCALL(glDeleteShader(fs));
	GLCALL(glDeleteShader(vs));

	return program;
}

int Shader::GetUniformLocation(const std::string & uniformname) const
{
	GLCALL(int location = glGetUniformLocation(m_RendererID, uniformname.c_str()) );
	if (location == -1)
	{
		std::cout << "warning: " << uniformname << " not found in the shader" << std::endl;
	}
	return location;
}
