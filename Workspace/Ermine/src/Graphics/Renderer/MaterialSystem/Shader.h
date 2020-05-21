#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<filesystem>
#include<fstream>

#include<glm.hpp>

namespace Ermine
{
	class Shader
	{
	public:
		Shader() = delete; //No Point In An Empty Shader..

		//Use This To Directly Give The Constructor Source Code..
		explicit Shader(std::string VertexShaderSource, std::string FragmentShaderSource); 

		//Use This To Give The Constructor A File From Which To Extract Source Code..
		explicit Shader(std::filesystem::path VertexShaderPath, std::filesystem::path FragmentShaderPath);

		~Shader();

	public:
		Shader(const Shader& rhs);
		Shader operator=(const Shader& rhs);
		
		Shader(Shader&& rhs);
		Shader operator=(Shader&& rhs);

		//Donot USe This It is Way Too Expensive..
		bool operator==(Shader& rhs);

		void Bind();
		void UnBind();

		std::string GetVertexShaderSource();
		std::string GetFragmentShaderSource();

		void UniformBool(std::string UniformName, bool Value);

		void Uniformf(std::string UniformName, float Value);
		void Uniform2f(std::string UniformName, glm::vec2 Value);
		void Uniform3f(std::string UniformName, glm::vec3 Value);
		void Uniform4f(std::string UniformName, glm::vec4 Value);
		void UniformNf(std::string UniformName, std::vector<float> Value);

		void Uniformi(std::string UniformName, int Value);
		void Uniform2i(std::string UniformName, glm::vec<2,int> Value);
		void Uniform3i(std::string UniformName, glm::vec<3,int> Value);
		void Uniform4i(std::string UniformName, glm::vec<4,int> Value);
		void UniformNi(std::string UniformName, std::vector<int> Value);

		void UniformMat3(std::string UniformName, glm::mat3 Matrix);
		void UniformMat4(std::string UniformName, glm::mat4 Matrix);

	public:

	protected:

	protected:

	private:
		void HelperCompileAndLinkShaderUsingSourceFilesInsideTheObject();
		void HelperCheckShaderForCompilationProblems(int Shader,std::string Name);

	private:
		std::string VertexShaderSource;
		std::string FragmentShaderSource;
		
		int ShaderProgram;
	};
}