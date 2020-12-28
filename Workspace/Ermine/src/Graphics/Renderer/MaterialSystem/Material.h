#pragma once
#include<iostream>
#include<vector>
#include<string>

#include <nlohmann/json.hpp>
#include "Shader.h"
#include <glm.hpp>

#include  "EngineResourceHandlers/GlobalTextureCache.h"
#include "Texture.h"

namespace Ermine
{
	class Material
	{
	public:
		//Material Can Be Empty But It Wont Compile..
		Material(); 
		
		//Material As Located On The Disk..
		Material(std::filesystem::path MaterialJsonFilePath);

		//Material From Shader Paths
		Material(std::filesystem::path VertexPath, std::filesystem::path FragmentPath);

		//Material Using Source Code..
		Material(std::string VertexSource, std::string FragmentSource);

		~Material();
	public:
		void Bind();

		std::shared_ptr<Ermine::Shader> GetShader();
		void SetShader(std::shared_ptr<Ermine::Shader> Shd);

		static void WriteMaterialToFile(Ermine::Material* Mat,std::filesystem::path FilePathToWrite);
		void WriteMaterialToFile(std::filesystem::path FilePathToWrite);

		static std::shared_ptr<Ermine::Material> Generate();
		static std::shared_ptr<Ermine::Material> Generate(std::filesystem::path MaterialPath);
		static std::shared_ptr<Ermine::Material> Generate(std::filesystem::path VertexShaderPath, std::filesystem::path FragmentShaderPath);
		static std::shared_ptr<Ermine::Material> Generate(std::string VertexShaderSource, std::string FragmentShaderSource);

		void ClearUniforms();
	public:

	protected:

	protected:

	private:

		void HelperCopyBuffersFunction(const Material& rhs);
		void HelperMoveBuffersFunction(Material&& rhs);

		void LoadUniformsFromMemory(std::filesystem::path JsonFile);
	private:
		std::vector<std::pair<std::string, int>> U1B;

		//Float Buffers
		std::vector<std::pair<std::string, float>>	   U1Float;				
		std::vector<std::pair<std::string, glm::vec2>> U2Float;			
		std::vector<std::pair<std::string, glm::vec3>> U3Float;			
		std::vector<std::pair<std::string, glm::vec4>> U4Float;			
		std::vector<std::pair<std::string, std::vector<float>>> UNFloat;

		//Int Buffer
		std::vector<std::pair<std::string, int>>			  U1Int;
		std::vector<std::pair<std::string, glm::vec<2,int>>>  U2Int;
		std::vector<std::pair<std::string, glm::vec<3,int>>>  U3Int;
		std::vector<std::pair<std::string, glm::vec<4,int>>>  U4Int;
		std::vector<std::pair<std::string, std::vector<int>>> UNInt;
		
		//Matrix Buffers
		std::vector<std::pair<std::string,glm::mat3>> U3Mat;
		std::vector<std::pair<std::string,glm::mat4>> U4Mat;

		//Material May Only Be Different On Uniforms Right..
		std::shared_ptr<Ermine::Shader> Shd = nullptr;
	};
}