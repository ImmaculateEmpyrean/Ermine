#include "stdafx.h"
#include "Shader.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "../OpenGLErrorChecker.h"

namespace Ermine
{
	Shader::Shader(std::string VertexShaderSource, std::string FragmentShaderSource)
		:
		VertexShaderSource(VertexShaderSource),
		FragmentShaderSource(FragmentShaderSource)
	{
		HelperCompileAndLinkShaderUsingSourceFilesInsideTheObject();
	}
	Shader::Shader(std::filesystem::path VertexShaderPath, std::filesystem::path FragmentShaderPath)
	{
		std::ifstream VertexShaderFile(VertexShaderPath, std::ios::binary);
		std::ifstream FragmentShaderFile(FragmentShaderPath, std::ios::binary);

		std::string Line;
		while (std::getline(VertexShaderFile, Line))
			VertexShaderSource = VertexShaderSource + Line;
		while (std::getline(FragmentShaderFile, Line))
			FragmentShaderSource = FragmentShaderSource + Line;

		HelperCompileAndLinkShaderUsingSourceFilesInsideTheObject();
	}

	Shader::~Shader()
	{
		glDeleteProgram(ShaderProgram);
	}


	Shader::Shader(const Shader& rhs)
		:
		VertexShaderSource(VertexShaderSource),
		FragmentShaderSource(FragmentShaderSource)
	{
		HelperCompileAndLinkShaderUsingSourceFilesInsideTheObject();
	}
	Shader Shader::operator=(const Shader& rhs)
	{
		VertexShaderSource = rhs.VertexShaderSource;
		FragmentShaderSource = rhs.FragmentShaderSource;

		HelperCompileAndLinkShaderUsingSourceFilesInsideTheObject();

		return *this;
	}

	Shader::Shader(Shader&& rhs)
		:
		VertexShaderSource(rhs.VertexShaderSource),
		FragmentShaderSource(rhs.FragmentShaderSource),
		ShaderProgram(rhs.ShaderProgram)
	{
		rhs.ShaderProgram = 0;
	}
	Shader Shader::operator=(Shader&& rhs)
	{
		VertexShaderSource = rhs.VertexShaderSource;
		FragmentShaderSource = rhs.FragmentShaderSource;
		ShaderProgram = rhs.ShaderProgram;

		rhs.ShaderProgram = 0;

		return *this;
	}

	bool Shader::operator==(Shader& rhs)
	{
		if (FragmentShaderSource == rhs.FragmentShaderSource)
		{
			if (VertexShaderSource == rhs.VertexShaderSource)
				return true;
			else return false;
		}
		else return false;
	}


	void Shader::Bind()
	{
		GLCall(glUseProgram(ShaderProgram));
	}
	void Shader::UnBind()
	{
		GLCall(glUseProgram(0));
	}
	
	
	std::string Shader::GetVertexShaderSource()
	{
		return VertexShaderSource;
	}
	std::string Shader::GetFragmentShaderSource()
	{
		return FragmentShaderSource;
	}


	void Shader::HelperCompileAndLinkShaderUsingSourceFilesInsideTheObject()
	{
		const char* c_str = VertexShaderSource.c_str();

		int VertexShader = glCreateShader(GL_VERTEX_SHADER);
		GLCall(glShaderSource(VertexShader, 1, &c_str, NULL));
		GLCall(glCompileShader(VertexShader));

		HelperCheckShaderForCompilationProblems(VertexShader, "VertexShader");

		c_str = FragmentShaderSource.c_str();

		int FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		GLCall(glShaderSource(FragmentShader, 1, &c_str, NULL));
		GLCall(glCompileShader(FragmentShader));

		HelperCheckShaderForCompilationProblems(FragmentShader, "FragmentShader");

		ShaderProgram = glCreateProgram();
		glAttachShader(ShaderProgram, VertexShader);
		glAttachShader(ShaderProgram, FragmentShader);
		glLinkProgram(ShaderProgram);

		int Success;
		char infoLog[512];
		glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
		if (!Success) {
			glGetProgramInfoLog(ShaderProgram, 512, NULL, infoLog);
			STDOUTLog_Error("Error Shader Program Linking Failed! Info Log : {}",infoLog);
		}
		glDeleteShader(VertexShader);
		glDeleteShader(FragmentShader);

	}
	void Shader::HelperCheckShaderForCompilationProblems(int Shader,std::string ShaderName)
	{
		int success;
		char infoLog[512];
		glGetShaderiv(Shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(Shader, 512, NULL, infoLog);
			STDOUTLog_Error("Error {} Shader Compilation Failed! Info Log : {}", ShaderName, infoLog);
		}
	}
}
