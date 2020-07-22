#include "stdafx.h"
#include "Material.h"

//#include "glm"
//#include <glm/gtc/type_ptr.hpp>

namespace Ermine
{
	Material::~Material()
	{
		if(Shd != nullptr)
			delete Shd;
	}


	Material::Material(const Material& rhs)
	{
		if (rhs.Shd != nullptr)
			Shd = new Ermine::Shader((*rhs.Shd));

		HelperCopyBuffersFunction(rhs);
	}
	Material Material::operator=(const Material& rhs)
	{
		if (rhs.Shd != nullptr)
			Shd = new Ermine::Shader((*rhs.Shd));

		HelperCopyBuffersFunction(rhs);

		return *this;
	}

	Material::Material(Material&& rhs)
	{
		if (rhs.Shd != nullptr)
		{
			Shd = rhs.Shd;
			rhs.Shd = nullptr;
		}

		HelperMoveBuffersFunction(std::move(rhs));
	}
	Material Material::operator=(Material&& rhs)
	{
		if (rhs.Shd != nullptr)
		{
			Shd = rhs.Shd;
			rhs.Shd = nullptr;
		}

		HelperMoveBuffersFunction(std::move(rhs));

		return *this;
	}


	bool Material::operator==(Material& rhs)
	{
		return false;
	}


	Material::Material(std::filesystem::path MaterialJsonFilePath)
	{
		std::ifstream RawInputFile(MaterialJsonFilePath);
		nlohmann::json InputFile;

		//Dump The Contents Of THe Raw Input File Into The Datastructure Designed For Parsing Json..
		InputFile << RawInputFile;

		std::string VertexShaderCode = InputFile["VertexShaderCode"];//.dump();
		std::string FragmentShaderCode = InputFile["FragmentShaderCode"];//.dump();

		Shd = new Shader(std::string(VertexShaderCode), std::string(FragmentShaderCode)); //Construct The Required Shader

		for (nlohmann::json::iterator it = InputFile["bool"].begin(); it != InputFile["bool"].end(); ++it) 
		{
			nlohmann::json FILE = *it;
			std::string UniformName = FILE.items().begin().key();
			int Value = std::stoi(FILE.items().begin().value().dump());

			UniformBoolean.emplace_back(std::make_pair(UniformName, Value));
		}

		for (nlohmann::json::iterator it = InputFile["float"].begin(); it != InputFile["float"].end(); ++it)
		{
			nlohmann::json FILE = *it;
			std::string UniformName = FILE.items().begin().key();
			float Value = std::stof(FILE.items().begin().value().dump());

			UniformfBuffer.emplace_back(std::make_pair(UniformName, Value));
		}

		for (nlohmann::json::iterator it = InputFile["Vec2"].begin(); it != InputFile["Vec2"].end(); ++it)
		{
			nlohmann::json FILE = *it;
			std::string UniformName = FILE.items().begin().key();
			
			std::vector<float> Vector;

			for (auto i : FILE.items().begin().value().items())
			{
				Vector.emplace_back(std::stof(i.value().dump()));
			}

			glm::vec2 Holder;
			Holder.x = Vector[0];
			Holder.y = Vector[1];

			Uniform2fBuffer.emplace_back(std::make_pair(UniformName, Holder));
		}

		for (nlohmann::json::iterator it = InputFile["Vec3"].begin(); it != InputFile["Vec3"].end(); ++it)
		{
			nlohmann::json FILE = *it;
			std::string UniformName = FILE.items().begin().key();

			std::vector<float> Vector;

			for (auto i : FILE.items().begin().value().items())
			{
				Vector.emplace_back(std::stof(i.value().dump()));
			}

			glm::vec3 Holder;
			Holder.x = Vector[0];
			Holder.y = Vector[1];
			Holder.z = Vector[2];

			Uniform3fBuffer.emplace_back(std::make_pair(UniformName, Holder));
		}

		for (nlohmann::json::iterator it = InputFile["Vec4"].begin(); it != InputFile["Vec4"].end(); ++it)
		{
			nlohmann::json FILE = *it;
			std::string UniformName = FILE.items().begin().key();

			std::vector<float> Vector;

			for (auto i : FILE.items().begin().value().items())
			{
				Vector.emplace_back(std::stof(i.value().dump()));
			}

			glm::vec4 Holder;
			Holder.x = Vector[0];
			Holder.y = Vector[1];
			Holder.z = Vector[2];
			Holder.w = Vector[3];

			Uniform4fBuffer.emplace_back(std::make_pair(UniformName, Holder));
		}

		for (nlohmann::json::iterator it = InputFile["VecN"].begin(); it != InputFile["VecN"].end(); ++it)
		{
			nlohmann::json FILE = *it;
			std::string UniformName = FILE.items().begin().key();

			std::vector<float> Vector;

			for (auto i : FILE.items().begin().value().items())
			{
				Vector.emplace_back(std::stof(i.value().dump()));
			}

			UniformNfBuffer.emplace_back(std::make_pair(UniformName, Vector));
		}


		//Start Loading Integer Uniforms From The File..

		for (nlohmann::json::iterator it = InputFile["int"].begin(); it != InputFile["int"].end(); ++it)
		{
			nlohmann::json FILE = *it;
			std::string UniformName = FILE.items().begin().key();
			int Value = std::stoi(FILE.items().begin().value().dump());

			UniformiBuffer.emplace_back(std::make_pair(UniformName, Value));
		}

		for (nlohmann::json::iterator it = InputFile["Vei2"].begin(); it != InputFile["Vei2"].end(); ++it)
		{
			nlohmann::json FILE = *it;
			std::string UniformName = FILE.items().begin().key();

			std::vector<int> Vector;

			for (auto i : FILE.items().begin().value().items())
			{
				Vector.emplace_back(std::stoi(i.value().dump()));
			}

			glm::vec<2,int> Holder;
			Holder.x = Vector[0];
			Holder.y = Vector[1];

			Uniform2iBuffer.emplace_back(std::make_pair(UniformName, Holder));
		}

		for (nlohmann::json::iterator it = InputFile["Vei3"].begin(); it != InputFile["Vei3"].end(); ++it)
		{
			nlohmann::json FILE = *it;
			std::string UniformName = FILE.items().begin().key();

			std::vector<int> Vector;

			for (auto i : FILE.items().begin().value().items())
			{
				Vector.emplace_back(std::stoi(i.value().dump()));
			}

			glm::vec<3,int> Holder;
			Holder.x = Vector[0];
			Holder.y = Vector[1];
			Holder.z = Vector[2];

			Uniform3iBuffer.emplace_back(std::make_pair(UniformName, Holder));
		}

		for (nlohmann::json::iterator it = InputFile["Vei4"].begin(); it != InputFile["Vei4"].end(); ++it)
		{
			nlohmann::json FILE = *it;
			std::string UniformName = FILE.items().begin().key();

			std::vector<int> Vector;

			for (auto i : FILE.items().begin().value().items())
			{
				Vector.emplace_back(std::stoi(i.value().dump()));
			}

			glm::vec<4,int> Holder;
			Holder.x = Vector[0];
			Holder.y = Vector[1];
			Holder.z = Vector[2];
			Holder.w = Vector[3];

			Uniform4iBuffer.emplace_back(std::make_pair(UniformName, Holder));
		}

		for (nlohmann::json::iterator it = InputFile["VeiN"].begin(); it != InputFile["VeiN"].end(); ++it)
		{
			nlohmann::json FILE = *it;
			std::string UniformName = FILE.items().begin().key();

			std::vector<int> Vector;

			for (auto i : FILE.items().begin().value().items())
			{
				Vector.emplace_back(std::stoi(i.value().dump()));
			}

			UniformNiBuffer.emplace_back(std::make_pair(UniformName, Vector));
		}

		//Ended Loading Integer Uniforms From The File..

		//Started Loading Matrix Buffers
		for (nlohmann::json::iterator it = InputFile["Mat3"].begin(); it != InputFile["Mat3"].end(); ++it)
		{
			nlohmann::json FILE = *it;
			std::string UniformName = FILE.items().begin().key();

			std::vector<int> Vector;

			for (auto i : FILE.items().begin().value().items())
			{
				Vector.emplace_back(std::stof(i.value().dump()));
			}

			glm::mat3 Matrix = glm::make_mat3(&Vector[0]);

			UniformMat3Buffer.emplace_back(std::make_pair(UniformName, Matrix));
		}

		for (nlohmann::json::iterator it = InputFile["Mat4"].begin(); it != InputFile["Mat4"].end(); ++it)
		{
			nlohmann::json FILE = *it;
			std::string UniformName = FILE.items().begin().key();

			std::vector<int> Vector;

			for (auto i : FILE.items().begin().value().items())
			{
				Vector.emplace_back(std::stof(i.value().dump()));
			}

			glm::mat4 Matrix = glm::make_mat4(&Vector[0]);

			UniformMat4Buffer.emplace_back(std::make_pair(UniformName, Matrix));
		}
		//Ended Loading Matrix Buffers
	}

	void Material::Bind()
	{
		if (Shd == nullptr)
			return;

		Shd->Bind();
		
		for (auto i : UniformBoolean)
			Shd->UniformBool(i.first, i.second);

		for (auto i : UniformfBuffer)
			Shd->Uniformf(i.first, i.second);
		for (auto i : Uniform2fBuffer)
			Shd->Uniform2f(i.first, i.second);
		for (auto i : Uniform3fBuffer)
			Shd->Uniform3f(i.first, i.second);
		for (auto i : Uniform4fBuffer)
			Shd->Uniform4f(i.first, i.second);
		for (auto i : UniformNfBuffer)
			Shd->UniformNf(i.first, i.second);
			
		for (auto i : UniformiBuffer)
			Shd->Uniformi(i.first, i.second);
		for (auto i : Uniform2iBuffer)
			Shd->Uniform2i(i.first, i.second);
		for (auto i : Uniform3iBuffer)
			Shd->Uniform3i(i.first, i.second);
		for (auto i : Uniform4iBuffer)
			Shd->Uniform4i(i.first, i.second);
		for (auto i : UniformNiBuffer)
			Shd->UniformNi(i.first, i.second);

		for (auto i : UniformMat3Buffer)
			Shd->UniformMat3(i.first, i.second);
		for (auto i : UniformMat4Buffer)
			Shd->UniformMat4(i.first, i.second);
	}

	Shader* Material::GetShader()
	{
		return Shd;
	}

	void Material::SetShader(Shader Shd)
	{
		this->Shd = new Shader(std::move(Shd));
	}

	void Material::WriteToFile(std::filesystem::path FilePathToWrite)
	{
		//This Function Will Be Popuated When There Is An Actual Ui To Create And Modify Materials.. Until Then There is No Need For This Method..
	}


	void Material::HelperCopyBuffersFunction(const Material& rhs)
	{
		UniformBoolean = rhs.UniformBoolean;

		UniformfBuffer  = rhs.UniformfBuffer;
		Uniform2fBuffer = rhs.Uniform2fBuffer;
		Uniform3fBuffer = rhs.Uniform3fBuffer;
		Uniform4fBuffer = rhs.Uniform4fBuffer;
		UniformNfBuffer = rhs.UniformNfBuffer;

		UniformiBuffer = rhs.UniformiBuffer;
		Uniform2iBuffer = rhs.Uniform2iBuffer;
		Uniform3iBuffer = rhs.Uniform3iBuffer;
		Uniform4iBuffer = rhs.Uniform4iBuffer;
		UniformNiBuffer = rhs.UniformNiBuffer;

		UniformMat3Buffer = rhs.UniformMat3Buffer;
		UniformMat4Buffer = rhs.UniformMat4Buffer;
	}

	void Material::HelperMoveBuffersFunction(Material&& rhs)
	{
		UniformBoolean = std::move(rhs.UniformBoolean);

		UniformfBuffer  = std::move(rhs.UniformfBuffer);
		Uniform2fBuffer = std::move(rhs.Uniform2fBuffer);
		Uniform3fBuffer = std::move(rhs.Uniform3fBuffer);
		Uniform4fBuffer = std::move(rhs.Uniform4fBuffer);
		UniformNfBuffer = std::move(rhs.UniformNfBuffer);

		UniformiBuffer  = std::move(rhs.UniformiBuffer);
		Uniform2iBuffer = std::move(rhs.Uniform2iBuffer);
		Uniform3iBuffer = std::move(rhs.Uniform3iBuffer);
		Uniform4iBuffer = std::move(rhs.Uniform4iBuffer);
		UniformNiBuffer = std::move(rhs.UniformNiBuffer);

		UniformMat3Buffer = std::move(rhs.UniformMat3Buffer);
		UniformMat4Buffer = std::move(rhs.UniformMat4Buffer);
	}
}
