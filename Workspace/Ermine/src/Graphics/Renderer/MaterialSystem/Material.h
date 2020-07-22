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
		//Material Can Be Empty For Now
		Material() = default; 
		//Material As Located On The Disk..
		Material(std::filesystem::path MaterialJsonFilePath);

		~Material();
	public:
		Material(const Material& rhs);
		Material operator=(const Material& rhs);

		Material(Material&& rhs);
		Material operator=(Material&& rhs);

		bool operator==(Material& rhs); //This Checks If Both The Shaders And The Uniforms Are Equal..

		//This Function Binds The Relevant Shader And Pushes The Relevant Uniforms So That No External Thing Can Worry Abt it..//
		void Bind();

		Shader* GetShader();
		void SetShader(Shader Shd);

		//The FilePath And the filename together sp that the material can be written to the disk
		void WriteToFile(std::filesystem::path FilePathToWrite);

	public:

	protected:

	protected:

	private:
		void HelperCopyBuffersFunction(const Material& rhs);
		void HelperMoveBuffersFunction(Material&& rhs);

	private:
		//Start Buffers//
		std::vector<std::pair<std::string, int>> UniformBoolean;

		//Float Buffers
		std::vector<std::pair<std::string, float>> UniformfBuffer;				
		std::vector<std::pair<std::string, glm::vec2>> Uniform2fBuffer;			
		std::vector<std::pair<std::string, glm::vec3>> Uniform3fBuffer;			
		std::vector<std::pair<std::string, glm::vec4>> Uniform4fBuffer;			
		std::vector<std::pair<std::string, std::vector<float>>> UniformNfBuffer;

		//Int Buffer
		std::vector<std::pair<std::string, int>> UniformiBuffer;
		std::vector<std::pair<std::string, glm::vec<2,int>>> Uniform2iBuffer;
		std::vector<std::pair<std::string, glm::vec<3,int>>> Uniform3iBuffer;
		std::vector<std::pair<std::string, glm::vec<4,int>>> Uniform4iBuffer;
		std::vector<std::pair<std::string, std::vector<int>>> UniformNiBuffer;
		
		//Matrix Buffers
		std::vector<std::pair<std::string,glm::mat3>> UniformMat3Buffer;
		std::vector<std::pair<std::string,glm::mat4>> UniformMat4Buffer;

		//Ended Buffers//
		Ermine::Shader* Shd = nullptr;
	};
}