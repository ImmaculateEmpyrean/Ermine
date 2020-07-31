#pragma once

/*
	A Label Is Used To Primarily Draw Text, U Submit a Label To a Layer Which When Parsed By The Renderer Draws Text..
*/

#include<iostream>
#include<vector>
#include<string>
#include<filesystem>

#include<algorithm>
#include<memory>

#include "glm.hpp"
#include "Graphics/Renderer/RenderableComponents/RenderableTextureModule.h"
#include "2DPrimitives/Constructs/MovableObject.h"

#include "2DPrimitives/Constructs/VertexTextured.h"

#include "EngineResourceHandlers/GlobalFontCache.h"
#include "FontRenderingSystem/Font.h"

namespace Ermine
{
	class Label : public RenderableTextureModule , public MovableObject
	{
	public:
		//An Empty Label Makes No Sense Though It Could Exist I Suppose
		Label() = default;

		//This Is The Most Basic Of Constructors.. And Realistically It Is Better If U Dont Use It I Think..
		Label(std::string Text, glm::vec3 Color = glm::vec3(1.0f,1.0f,1.0f));

		//For Special Cases
		Label(std::string Text, glm::mat4 ModelMatrix, glm::vec3 Color = glm::vec3(1.0f, 1.0f, 1.0f));

		//This Is One Of The Set Of Constructors Ur Likely To Use
		Label(std::string Text, glm::vec3 Color, std::shared_ptr<Font> fnt						 , glm::mat4 ModelMatrix = glm::mat4(1.0f));
		Label(std::string Text, glm::vec3 Color, std::filesystem::path FontFilePath, int FontSize, glm::mat4 ModelMatrix = glm::mat4(1.0f));
		Label(std::string Text, glm::vec3 Color, std::string FontName			   , int FontSize, glm::mat4 ModelMatrix = glm::mat4(1.0f));


	public:
		//Clears The Text..
		void ClearText();

		//Clear And Write New Color
		void SetTextColor(glm::vec3 TextColor);

		//Clear And Write New Text
		void ClearAndWriteText(std::string Text);

		//Append Text To Various Different Positions
		void AppendTextToBack(std::string Text);
		void AppendTextToBegin(std::string Text);

	public:

	protected:

	protected:

	private:
		//This Is By Far The Most Important Method In Label.h It Is Used To Actually Instantiate All Components Of The LAbel To Full Working Order.. See That The Actual Variables Rewuired Are In Proper States..
		void HelperConstructLabel();

		//This Overrides The Default Behaviour Of The Renderable Texture Module And Actually Binds The Font Texture Its Private Nobody ever Needs To Know About It..
		virtual std::vector<int> BindTexturesContained() override;

	private:
		//The Font Is Not Owned By The Label And Hence Is Not Deleted By The Label Per Se..
		std::shared_ptr<Font> fnt;

		//The Text The Label Is Supposed To Display Really Not Required But Thought Lets Just Keep It Around..
		std::string Text;

		//The Text Color Now This Is An Important Variable And Is Required To Be Stored..
		glm::vec3 TextColor;
	};
}