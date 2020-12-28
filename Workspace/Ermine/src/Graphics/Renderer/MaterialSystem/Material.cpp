#include "stdafx.h"
#include "Material.h"

//#include "glm"
//#include <glm/gtc/type_ptr.hpp>

namespace Ermine
{
	Material::Material()
	{
		//An Empty Shader Wont Compile..
		Shd = Ermine::Shader::Generate();
	}

	Material::Material(std::filesystem::path VertexPath, std::filesystem::path FragmentPath)
	{
		Shd = Ermine::Shader::Generate(VertexPath, FragmentPath);
	}
	Material::Material(std::string VertexSource, std::string FragmentSource)
	{
		Shd = Ermine::Shader::Generate(VertexSource, FragmentSource);
	}
	Material::~Material()
	{}


	template<typename T>
	class StringToDatatype
	{};
	template<>
	class StringToDatatype<int>
	{
	public:
		int operator()(std::string Value)
		{
			return std::stoi(Value);
		}
	};
	template<>
	class StringToDatatype<float>
	{
	public:
		float operator()(std::string Value)
		{
			return std::stof(Value);
		}
	};
	template<>
	class StringToDatatype<bool>
	{
	public:
		float operator()(std::string Value)
		{
			return (Value == std::string("true")) ? 1 : 0;
		}
	};

	//General Purpose Function Overload
	template<typename T,typename Q>
	static std::vector<std::pair<std::string, T>> GetUniform(nlohmann::json File, std::string UniformType,StringToDatatype<Q> Functor)
	{
		std::vector<std::pair<std::string, T>> UniformBuffer;

		for (auto Uniform : File[UniformType.c_str()].items())
		{
			std::pair<std::string, T> IdValuePair;
			for (auto Parameter : Uniform.value().items())
			{
				if (Parameter.key() == "Name")
					IdValuePair.first = Parameter.value();

				if (Parameter.key() == "Value")
				{
					int Counter = 0;
					int SecondaryCounter = 0;
					for (auto Val : Parameter.value().items())
					{
						//Just Normally Write The Values For Int Float And bool
						if (std::is_same<T, int>::value || std::is_same<T, float>::value || std::is_same<T, bool>::value)
							IdValuePair.second = Functor(Val.value().dump());

						//This is How std::vectors Are Handled..
						else if (std::is_same<T, std::vector<float>>::value || std::is_same<T, std::vector<int>>::value)
							IdValuePair.second.emplace_back(Functor(Val.value().dump()));
						
						//This Is How Matrices Are Handled
						else if (std::is_same<T, glm::mat3>::value || std::is_same<T, glm::mat4>::value)
						{
							int TerminationPoint = -99;

							if (std::is_same<T, glm::mat3>::value)
								TerminationPoint == 3;
							else TerminationPoint == 4;

							if (SecondaryCounter == TerminationPoint)
							{
								SecondaryCounter = 0;
								Counter++;
							}
							IdValuePair.second[Counter][SecondaryCounter++] = Functor(Val.value().dump());
						}

						//Only glm::vec is left To Be Handled And It Can Be Handled Like This
						else IdValuePair.second[Counter++] = Functor(Val.value().dump());
					}	
				}
			}
			UniformBuffer.emplace_back(IdValuePair);
		}

		return std::move(UniformBuffer);
	}

	void Material::ClearUniforms()
	{
		U1B.clear();

		U1Float.clear();
		U2Float.clear();
		U3Float.clear();
		U4Float.clear();
		UNFloat.clear();

		U1Int.clear();
		U2Int.clear();
		U3Int.clear();
		U4Int.clear();
		UNInt.clear();

		U3Mat.clear();
		U4Mat.clear();
	}

	void Material::LoadUniformsFromMemory(std::filesystem::path JsonFilePath)
	{
		std::ifstream RawInputFile(JsonFilePath);

		nlohmann::json File;
		File << RawInputFile;

		U1B = GetUniform<int>(File, "U1B", Ermine::StringToDatatype<bool>{});

		U1Float = GetUniform<float>	   (File, "U1", StringToDatatype<float>{});
		U2Float = GetUniform<glm::vec2>(File, "U2", StringToDatatype<float>{});
		U3Float = GetUniform<glm::vec3>(File, "U3", StringToDatatype<float>{});
		U4Float = GetUniform<glm::vec4>(File, "U4F", StringToDatatype<float>{});
		UNFloat = GetUniform<std::vector<float>>(File, "UNF", StringToDatatype<float>{});

		U1Int = GetUniform<int>				(File, "U1I", StringToDatatype<int>{});
		U2Int = GetUniform<glm::vec<2, int>>(File, "U2I", StringToDatatype<int>{});
		U3Int = GetUniform<glm::vec<3, int>>(File, "U3I", StringToDatatype<int>{});
		U4Int = GetUniform<glm::vec<4, int>>(File, "U4I", StringToDatatype<int>{});
		UNInt = GetUniform<std::vector<int>>(File, "UNI", StringToDatatype<int>{});

		U3Mat = GetUniform<glm::mat3>(File, "U3Mat", StringToDatatype<float>{});
		U4Mat = GetUniform<glm::mat4>(File, "U4Mat", StringToDatatype<float>{});
	}

	Material::Material(std::filesystem::path MaterialJsonFilePath)
	{
		std::ifstream RawInputFile(MaterialJsonFilePath);

		nlohmann::json File;
		File << RawInputFile;

		std::string VertexSource   = File["Vertex"];
		std::string FragmentSource = File["Fragment"];

		//Load All The Uniforms From Memory..
		LoadUniformsFromMemory(MaterialJsonFilePath);
	}

	

	void Material::Bind()
	{
		if (Shd == nullptr)
			return;

		Shd->Bind();
		
		for (auto i : U1B)
			Shd->UniformBool(i.first, i.second);

		for (auto i : U1Float)
			Shd->Uniformf(i.first, i.second);
		for (auto i : U2Float)
			Shd->Uniform2f(i.first, i.second);
		for (auto i : U3Float)
			Shd->Uniform3f(i.first, i.second);
		for (auto i : U4Float)
			Shd->Uniform4f(i.first, i.second);
		for (auto i : UNFloat)
			Shd->UniformNf(i.first, i.second);
			
		for (auto i : U1Int)
			Shd->Uniformi(i.first, i.second);
		for (auto i : U2Int)
			Shd->Uniform2i(i.first, i.second);
		for (auto i : U3Int)
			Shd->Uniform3i(i.first, i.second);
		for (auto i : U4Int)
			Shd->Uniform4i(i.first, i.second);
		for (auto i : UNInt)
			Shd->UniformNi(i.first, i.second);

		for (auto i : U3Mat)
			Shd->UniformMat3(i.first, i.second);
		for (auto i : U4Mat)
			Shd->UniformMat4(i.first, i.second);
	}


	std::shared_ptr<Ermine::Shader> Material::GetShader()
	{
		return Shd;
	}
	void Material::SetShader(std::shared_ptr<Ermine::Shader>Shd)
	{
		this->Shd = Shd;
	}


	template<typename T>
	static nlohmann::json SerializeUniform(std::string UnifromName, T Uniform)
	{
		nlohmann::json SerializedJson;
		SerializedJson["Name"] = UniformName;


		if (std::is_same<T, bool>::value)
			SerializedJson["Value"].emplace_back((Uniform == true)?"true" :"false");
		else if (std::is_same<T, int>::value || std::is_same<T, float>::value)
			SerializedJson["Value"].emplace_back(Uniform);
		else if (std::is_same<T, glm::vec2>::value || std::is_same<T, glm::vec<2,int>>::value)
		{
			SerializedJson["Value"].emplace_back(Uniform[0]);
			SerializedJson["Value"].emplace_back(Uniform[1]);
		}
		else if (std::is_same<T, glm::vec3>::value || std::is_same<T, glm::vec<3, int>>::value)
		{
			SerializedJson["Value"].emplace_back(Uniform[0]);
			SerializedJson["Value"].emplace_back(Uniform[1]);
			SerializedJson["Value"].emplace_back(Uniform[2]);
		}
		else if (std::is_same<T, glm::vec4>::value || std::is_same<T, glm::vec<4, int>>::value)
		{
			SerializedJson["Value"].emplace_back(Uniform[0]);
			SerializedJson["Value"].emplace_back(Uniform[1]);
			SerializedJson["Value"].emplace_back(Uniform[2]);
			SerializedJson["Value"].emplace_back(Uniform[3]);
		}
		else if (std::is_same<T, glm::vec2>::value || std::is_same<T, glm::vec<2, int>>::value)
		{
			SerializedJson["Value"].emplace_back(Uniform[0]);
			SerializedJson["Value"].emplace_back(Uniform[1]);
		}
		else if (std::is_same<T, std::vector<float>>::value || std::is_same<T, std::vector<int>>::value)
		{
			for(auto i : Uniform)
				SerializedJson["Value"].emplace_back(i);
		}
		else if (std::is_same<T, glm::mat3>::value || std::is_same<T, glm::mat4>::value)
		{
			int TerminationPoint = -99;
			if (std::is_same<T, glm::mat3>::value)
				TerminationPoint = 3;
			else if (std::is_same<T, glm::mat4>::value)
				TerminationPoint = 4;
			for (int i = 0; i < TerminationPoint; i++)
			{
				for(int j=0;j<TerminationPoint;j++)
					SerializedJson["Value"].emplace_back(Uniform[i][j]);
			}
		}
		return SerializedJson;
	}

	void Material::WriteMaterialToFile(Ermine::Material* Mat, std::filesystem::path FilePathToWrite)
	{
		nlohmann::json File;

		File["Vertex"]   = Mat->Shd->GetVertexShaderSource();
		File["Fragment"] = Mat->Shd->GetVertexShaderSource();

		for (auto i : Mat->U1B)
			File["U1B"].emplace_back(SerializeUniform(i.first, i.second));

		for (auto i : Mat->U1Float)
			File["U1F"].emplace_back(SerializeUniform(i.first, i.second));
		for (auto i : Mat->U2Float)
			File["U2F"].emplace_back(SerializeUniform(i.first, i.second));
		for (auto i : Mat->U3Float)
			File["U3F"].emplace_back(SerializeUniform(i.first, i.second));
		for (auto i : Mat->U4Float)
			File["U4F"].emplace_back(SerializeUniform(i.first, i.second));
		for (auto i : Mat->UNFloat)
			File["UNF"].emplace_back(SerializeUniform(i.first, i.second));

		for (auto i : Mat->U1Int)
			File["U1I"].emplace_back(SerializeUniform(i.first, i.second));

		for (auto i : Mat->U2Int)
			File["U2I"].emplace_back(SerializeUniform(i.first, i.second));
		for (auto i : Mat->U3Int)
			File["U3I"].emplace_back(SerializeUniform(i.first, i.second));
		for (auto i : Mat->U4Int)
			File["U4I"].emplace_back(SerializeUniform(i.first, i.second));
		for (auto i : Mat->UNInt)
			File["UNI"].emplace_back(SerializeUniform(i.first, i.second));

		for (auto i : Mat->U3Mat)
			File["U3Mat"].emplace_back(SerializeUniform(i.first, i.second));
		for (auto i : Mat->U4Mat)
			File["U4Mat"].emplace_back(SerializeUniform(i.first, i.second));

		std::ofstream FileOutput(FilePathToWrite);

		File >> FileOutput;
		FileOutput.close();
	}
	void Material::WriteMaterialToFile(std::filesystem::path FilePathToWrite)
	{
		Ermine::Material::WriteMaterialToFile(this, FilePathToWrite);
	}

#pragma region GenerateFunctions
	std::shared_ptr<Ermine::Material> Material::Generate()
	{
		return std::shared_ptr<Ermine::Material>(new Ermine::Material());
	}
	std::shared_ptr<Ermine::Material> Material::Generate(std::filesystem::path MaterialPath)
	{
		return std::shared_ptr<Ermine::Material>(new Ermine::Material(MaterialPath));
	}
	std::shared_ptr<Ermine::Material> Material::Generate(std::filesystem::path VertexShaderPath, std::filesystem::path FragmentShaderPath)
	{
		return std::shared_ptr<Ermine::Material>(new Ermine::Material(VertexShaderPath,FragmentShaderPath));
	}
	std::shared_ptr<Ermine::Material> Material::Generate(std::string VertexShaderSource, std::string FragmentShaderSource)
	{
		return std::shared_ptr<Ermine::Material>(new Ermine::Material(VertexShaderSource,FragmentShaderSource));
	}
#pragma endregion

	void Material::HelperCopyBuffersFunction(const Material& rhs)
	{
		U1B = rhs.U1B;

		U1Float = rhs.U1Float;
		U2Float = rhs.U2Float;
		U3Float = rhs.U3Float;
		U4Float = rhs.U4Float;
		UNFloat = rhs.UNFloat;

		U1Int = rhs.U1Int;
		U2Int = rhs.U2Int;
		U3Int = rhs.U3Int;
		U4Int = rhs.U4Int;
		UNInt = rhs.UNInt;

		U3Mat = rhs.U3Mat;
		U4Mat = rhs.U4Mat;
	}
	void Material::HelperMoveBuffersFunction(Material&& rhs)
	{
		U1B = std::move(rhs.U1B);

		U1Float = std::move(rhs.U1Float);
		U2Float = std::move(rhs.U2Float);
		U3Float = std::move(rhs.U3Float);
		U4Float = std::move(rhs.U4Float);
		UNFloat = std::move(rhs.UNFloat);

		U1Int = std::move(rhs.U1Int);
		U2Int = std::move(rhs.U2Int);
		U3Int = std::move(rhs.U3Int);
		U4Int = std::move(rhs.U4Int);
		UNInt = std::move(rhs.UNInt);

		U3Mat = std::move(rhs.U3Mat);
		U4Mat = std::move(rhs.U4Mat);
	}
}
