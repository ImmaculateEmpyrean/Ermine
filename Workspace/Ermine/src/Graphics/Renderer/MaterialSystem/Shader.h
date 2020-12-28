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
		//An Empty Shader Can Exist With Defaults.. But It Will Fail to Compile.. If Vertex And Fragment Shader Are Not Set.. 
		Shader() = default;

		//Use This To Directly Give The Constructor Source Code..
		Shader(std::string VertexShaderSource, std::string FragmentShaderSource); 

		//Use This To Give The Constructor A File From Which To Extract Source Code..
		Shader(std::filesystem::path VertexShaderPath, std::filesystem::path FragmentShaderPath);

		~Shader();

	public:
		Shader(const Shader& rhs);
		Shader operator=(const Shader& rhs);
		
		Shader(Shader&& rhs);
		Shader operator=(Shader&& rhs);

		void Bind();
		void UnBind();

		void SetVertexShaderSource(std::string VertexShaderSource);
		void SetVertexShaderSource(std::filesystem::path VertexShaderFile);
		std::string GetVertexShaderSource();

		void SeFragmentShaderSource(std::string FragmentShaderSource);
		void SetFragmentShaderSource(std::filesystem::path FragmentShaderFile);
		std::string GetFragmentShaderSource();

#pragma region GeneratorFunctions
		static std::shared_ptr<Ermine::Shader> Generate();
		static std::shared_ptr<Ermine::Shader> Generate(std::string VertexSource,std::string FragSource);
		static std::shared_ptr<Ermine::Shader> Generate(std::filesystem::path VertexPath, std::filesystem::path FragmentPath);
#pragma endregion

		void UniformBool(std::string UniformName, bool Value);

		void Uniformf (std::string UniformName, float Value);
		void Uniform2f(std::string UniformName, glm::vec2 Value);
		void Uniform3f(std::string UniformName, glm::vec3 Value);
		void Uniform4f(std::string UniformName, glm::vec4 Value);
		void UniformNf(std::string UniformName, std::vector<float> Value);

		void Uniformi (std::string UniformName, int Value);
		void Uniform2i(std::string UniformName, glm::vec<2,int> Value);
		void Uniform3i(std::string UniformName, glm::vec<3,int> Value);
		void Uniform4i(std::string UniformName, glm::vec<4,int> Value);
		void UniformNi(std::string UniformName, std::vector<int> Value);

		void UniformMat3(std::string UniformName, glm::mat3 Matrix);
		void UniformMat4(std::string UniformName, glm::mat4 Matrix);

	private:
		std::string VertexShaderSource   = "NULL";
		std::string FragmentShaderSource = "NULL";

		//This Bool Indicates Wether The Objetc Is Ready To Be Bound Or Not..
		bool ShaderReady = false;

		//Zero Means Currently No Program Is Set.. (Checked With The OpenGl Documentation.)
		int ShaderProgram = 0;
	};
}