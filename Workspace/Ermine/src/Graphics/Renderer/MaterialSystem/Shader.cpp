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
	{}

	std::string LoadShaderFromFile(std::filesystem::path ShaderPath)
	{
		std::ifstream ShaderFile(ShaderPath);
		std::string ShaderSource = "";

		while (ShaderFile)
		{
			char ch = ShaderFile.get();
			if (ShaderFile.good())
				ShaderSource = ShaderSource + ch;
		}

		return ShaderSource;
	}

	Shader::Shader(std::filesystem::path VertexShaderPath, std::filesystem::path FragmentShaderPath)
	{
		VertexShaderSource   = LoadShaderFromFile(VertexShaderPath);
		FragmentShaderSource = LoadShaderFromFile(FragmentShaderPath);
	}

	Shader::~Shader()
	{
		if(ShaderProgram != 0)
			glDeleteProgram(ShaderProgram);
	}


	Shader::Shader(const Shader& rhs)
		:
		VertexShaderSource(rhs.VertexShaderSource),
		FragmentShaderSource(rhs.FragmentShaderSource)
	{}
	Shader Shader::operator=(const Shader& rhs)
	{
		VertexShaderSource = rhs.VertexShaderSource;
		FragmentShaderSource = rhs.FragmentShaderSource;

		return *this;
	}

	Shader::Shader(Shader&& rhs)
		:
		VertexShaderSource(std::move(rhs.VertexShaderSource)),
		FragmentShaderSource(std::move(rhs.FragmentShaderSource)),
		ShaderProgram(rhs.ShaderProgram)
	{
		rhs.ShaderProgram = 0;

		//Intentionally Copying Over Shader Ready State..
		ShaderReady = rhs.ShaderReady;
	}
	Shader Shader::operator=(Shader&& rhs)
	{
		VertexShaderSource   = std::move(rhs.VertexShaderSource  );
		FragmentShaderSource = std::move(rhs.FragmentShaderSource);

		ShaderProgram = rhs.ShaderProgram;
		rhs.ShaderProgram = 0;

		ShaderReady = rhs.ShaderReady;

		return *this;
	}


	static void CompilationProblemChecker(int Shader, std::string ShaderName)
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

	void Shader::Bind()
	{
		if (ShaderReady == true)
		{
			GLCall(glUseProgram(ShaderProgram));
		}
		else
		{
			const char* c_str = VertexShaderSource.c_str();

			int VertexShader = glCreateShader(GL_VERTEX_SHADER);
			GLCall(glShaderSource(VertexShader, 1, &c_str, NULL));
			GLCall(glCompileShader(VertexShader));

			CompilationProblemChecker(VertexShader, "VertexShader");

			c_str = FragmentShaderSource.c_str();

			int FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			GLCall(glShaderSource(FragmentShader, 1, &c_str, NULL));
			GLCall(glCompileShader(FragmentShader));

			CompilationProblemChecker(FragmentShader, "FragmentShader");

			ShaderProgram = glCreateProgram();
			glAttachShader(ShaderProgram, VertexShader);
			glAttachShader(ShaderProgram, FragmentShader);
			glLinkProgram(ShaderProgram);

			int Success;
			char infoLog[512];
			glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
			if (!Success) {
				glGetProgramInfoLog(ShaderProgram, 512, NULL, infoLog);
				STDOUTLog_Error("Error Shader Program Linking Failed! Info Log : {}", infoLog);
			}
			glDeleteShader(VertexShader);
			glDeleteShader(FragmentShader);

			GLCall(glUseProgram(ShaderProgram));
			ShaderReady = true;
		}
	}
	void Shader::UnBind()
	{
		GLCall(glUseProgram(0));
	}


	void Shader::SetVertexShaderSource(std::string VertexShaderSource)
	{
		this->VertexShaderSource = VertexShaderSource;
		ShaderReady = false;
	}
	void Shader::SetVertexShaderSource(std::filesystem::path VertexShaderFile)
	{
		VertexShaderSource = LoadShaderFromFile(VertexShaderFile);
		ShaderReady = false;
	}
	std::string Shader::GetVertexShaderSource()
	{
		return VertexShaderSource;
	}
	
	void Shader::SeFragmentShaderSource(std::string FragmentShaderSource)
	{
		this->FragmentShaderSource = FragmentShaderSource;
		ShaderReady = false;
	}
	void Shader::SetFragmentShaderSource(std::filesystem::path FragmentShaderFile)
	{
		FragmentShaderSource = LoadShaderFromFile(FragmentShaderFile);
		ShaderReady = false;
	}
	std::string Shader::GetFragmentShaderSource()
	{
		return FragmentShaderSource;
	}


	std::shared_ptr<Ermine::Shader> Shader::Generate()
	{
		return std::shared_ptr<Ermine::Shader>();
	}
	std::shared_ptr<Ermine::Shader> Shader::Generate(std::string VertexSource, std::string FragSource)
	{
		return std::shared_ptr<Ermine::Shader>(new Ermine::Shader(VertexSource,FragSource));
	}
	std::shared_ptr<Ermine::Shader> Shader::Generate(std::filesystem::path VertexPath, std::filesystem::path FragmentPath)
	{
		return std::shared_ptr<Ermine::Shader>(new Ermine::Shader(VertexPath,FragmentPath));
	}


#pragma region UniformDefinitions
	void Shader::UniformBool(std::string UniformName, bool Value)
	{
		glUniform1i(glGetUniformLocation(ShaderProgram, UniformName.c_str()), (int)Value);
	}

	void Shader::Uniformf(std::string UniformName, float Value)
	{
		glUniform1f(glGetUniformLocation(ShaderProgram, UniformName.c_str()), Value);
	}

	void Shader::Uniform2f(std::string UniformName, glm::vec2 Value)
	{
		glUniform2f(glGetUniformLocation(ShaderProgram, UniformName.c_str()), Value.x,Value.y);
	}

	void Shader::Uniform3f(std::string UniformName, glm::vec3 Value)
	{
		glUniform3f(glGetUniformLocation(ShaderProgram, UniformName.c_str()), Value.x,Value.y,Value.z);
	}

	void Shader::Uniform4f(std::string UniformName, glm::vec4 Value)
	{
		glUniform4f(glGetUniformLocation(ShaderProgram, UniformName.c_str()), Value.x, Value.y, Value.z,Value.w);
	}

	void Shader::UniformNf(std::string UniformName, std::vector<float> Value)
	{
		glUniform1fv(glGetUniformLocation(ShaderProgram, UniformName.c_str()), Value.size(), &Value[0]);
	}

	void Shader::Uniformi(std::string UniformName, int Value)
	{
		glUniform1i(glGetUniformLocation(ShaderProgram, UniformName.c_str()), Value);
	}

	void Shader::Uniform2i(std::string UniformName, glm::vec<2, int> Value)
	{
		glUniform2i(glGetUniformLocation(ShaderProgram, UniformName.c_str()), Value.x, Value.y);
	}

	void Shader::Uniform3i(std::string UniformName, glm::vec<3, int> Value)
	{
		glUniform3i(glGetUniformLocation(ShaderProgram, UniformName.c_str()), Value.x, Value.y, Value.z);
	}

	void Shader::Uniform4i(std::string UniformName, glm::vec<4, int> Value)
	{
		glUniform4i(glGetUniformLocation(ShaderProgram, UniformName.c_str()), Value.x, Value.y, Value.z, Value.w);
	}

	void Shader::UniformNi(std::string UniformName, std::vector<int> Value)
	{
		glUniform1iv(glGetUniformLocation(ShaderProgram, UniformName.c_str()), Value.size(), &Value[0]);
	}

	void Shader::UniformMat3(std::string UniformName, glm::mat3 Matrix)
	{
		glUniformMatrix3fv(glGetUniformLocation(ShaderProgram, UniformName.c_str()), 1, GL_FALSE, glm::value_ptr(Matrix));
	}

	void Shader::UniformMat4(std::string UniformName, glm::mat4 Matrix)
	{
		glUniformMatrix4fv(glGetUniformLocation(ShaderProgram, UniformName.c_str()), 1, GL_FALSE, glm::value_ptr(Matrix));
	}
#pragma endregion UniformDefinitions
}
