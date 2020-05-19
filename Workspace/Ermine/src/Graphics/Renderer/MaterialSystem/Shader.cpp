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
		std::ifstream VertexShaderFile(VertexShaderPath);
		std::ifstream FragmentShaderFile(FragmentShaderPath);

		while (VertexShaderFile)
		{
			char ch = VertexShaderFile.get();
			if (VertexShaderFile.good())
				VertexShaderSource = VertexShaderSource + ch;
		}
		
		while (FragmentShaderFile)
		{
			char ch = FragmentShaderFile.get();
			if (FragmentShaderFile.good())
				FragmentShaderSource = FragmentShaderSource + ch;
		}

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

#pragma region UniformDefinitions
	void Shader::UniformBool(std::string& UniformName, bool Value)
	{
		glUniform1i(glGetUniformLocation(ShaderProgram, UniformName.c_str()), (int)Value);
	}

	void Shader::Uniformf(std::string& UniformName, float Value)
	{
		glUniform1f(glGetUniformLocation(ShaderProgram, UniformName.c_str()), Value);
	}

	void Shader::Uniform2f(std::string& UniformName, glm::vec2 Value)
	{
		glUniform2f(glGetUniformLocation(ShaderProgram, UniformName.c_str()), Value.x,Value.y);
	}

	void Shader::Uniform3f(std::string& UniformName, glm::vec3 Value)
	{
		glUniform3f(glGetUniformLocation(ShaderProgram, UniformName.c_str()), Value.x,Value.y,Value.z);
	}

	void Shader::Uniform4f(std::string& UniformName, glm::vec4 Value)
	{
		glUniform4f(glGetUniformLocation(ShaderProgram, UniformName.c_str()), Value.x, Value.y, Value.z,Value.w);
	}

	void Shader::UniformNf(std::string& UniformName, std::vector<float> Value)
	{
		glUniform1fv(glGetUniformLocation(ShaderProgram, UniformName.c_str()), Value.size(), &Value[0]);
	}

	void Shader::Uniformi(std::string& UniformName, int Value)
	{
		glUniform1i(glGetUniformLocation(ShaderProgram, UniformName.c_str()), Value);
	}

	void Shader::Uniform2i(std::string& UniformName, glm::vec<2, int> Value)
	{
		glUniform2i(glGetUniformLocation(ShaderProgram, UniformName.c_str()), Value.x, Value.y);
	}

	void Shader::Uniform3i(std::string& UniformName, glm::vec<3, int> Value)
	{
		glUniform3i(glGetUniformLocation(ShaderProgram, UniformName.c_str()), Value.x, Value.y, Value.z);
	}

	void Shader::Uniform4i(std::string& UniformName, glm::vec<4, int> Value)
	{
		glUniform4i(glGetUniformLocation(ShaderProgram, UniformName.c_str()), Value.x, Value.y, Value.z, Value.w);
	}

	void Shader::UniformNi(std::string& UniformName, std::vector<int> Value)
	{
		glUniform1iv(glGetUniformLocation(ShaderProgram, UniformName.c_str()), Value.size(), &Value[0]);
	}

	void Shader::UniformMat3(std::string& UniformName, glm::mat3 Matrix)
	{
		glUniformMatrix3fv(glGetUniformLocation(ShaderProgram, UniformName.c_str()), 1, GL_FALSE, glm::value_ptr(Matrix));
	}

	void Shader::UniformMat4(std::string& UniformName, glm::mat4 Matrix)
	{
		glUniformMatrix4fv(glGetUniformLocation(ShaderProgram, UniformName.c_str()), 1, GL_FALSE, glm::value_ptr(Matrix));
	}

#pragma endregion UniformDefinitions

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
