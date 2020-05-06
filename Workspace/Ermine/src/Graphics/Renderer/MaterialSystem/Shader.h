#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<filesystem>
#include<fstream>

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