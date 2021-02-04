#include "stdafx.h"
#include "Texture.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Graphics/Renderer/OpenGLErrorChecker.h"

#include "stb_image.h"

namespace Ermine
{
	enum class textureWrap
	{
		repeat			= GL_REPEAT,
		mirroredRepeat  = GL_MIRRORED_REPEAT,
		clampToEdge		= GL_CLAMP_TO_EDGE,
		ClampToBorder	= GL_CLAMP_TO_BORDER
	};
	enum class textureFilter
	{
		nearest = GL_NEAREST,
		linear	= GL_LINEAR,

		nearestPixelNearestMipMap = GL_NEAREST_MIPMAP_NEAREST,
		linearPixelNearestMipMap  = GL_LINEAR_MIPMAP_NEAREST,
		nearestPixelLinearMipMap  = GL_NEAREST_MIPMAP_LINEAR,
		linearPixelLinearMipMap   = GL_LINEAR_MIPMAP_LINEAR
	};


	struct textureGlobalStruct
	{
	public:
		void writeTextureToMap(std::filesystem::path texturePath,std::shared_ptr<Ermine::Texture> tex)
		{
			std::string texturePathString = texturePath.u8string();
			
			std::lock_guard<std::mutex> lock(textureMapMutex);
			filePathToIDMap[texturePathString] = tex;
			return;
		}
	private:
		std::mutex textureMapMutex;
		std::unordered_map<std::string, std::shared_ptr<Ermine::Texture>> filePathToIDMap;
	};

	Texture::Texture(std::filesystem::path textureFilePath, std::string TEXName)
	{
		GL_MIRRORED_REPEAT
	}
	Texture::~Texture()
	{}


}