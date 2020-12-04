#include "stdafx.h"
#include "TiledActor2D.h"

#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>

#define SOL_ALL_SAFETIES_ON 1
#include "sol/sol.hpp"

#include "sol/assert.hpp"
#include <iostream>

inline int my_add(int x, int y) {
	return x + y;
}

struct multiplier {
	int operator()(int x) {
		return x * 10;
	}

	static int by_five(int x) {
		return x * 5;
	}
};

namespace Ermine
{
	TiledActor2D::TiledActor2D(std::filesystem::path TmxFilePath)
		:
		ImageBase()
	{
		/*tmx::Map map;
		if (map.load("TileMaps/TestMap.tmx"))
		{
			//Initilaize
			std::vector<std::vector<std::shared_ptr<Ermine::Sprite>>> Tilesets;
			
			const auto& tilesets = map.getTilesets();
			for (const auto& tileset : tilesets)
			{
				//read out tile set properties, load textures etc...
				
			}

			const auto& layers = map.getLayers();
			for (const auto& layer : layers)
			{
				if (layer->getType() == tmx::Layer::Type::Object)
				{
					const auto& objectLayer = layer->getLayerAs<tmx::ObjectGroup>();
					const auto& objects = objectLayer.getObjects();
					for (const auto& object : objects)
					{
						//do stuff with object properties
					}
				}
				else if (layer->getType() == tmx::Layer::Type::Tile)
				{
					const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
					//read out tile layer properties etc...
					
					for (auto Tile : tileLayer.getTiles())
					{
						//Read Out The Tiles And Stuff..
						std::cout<<Tile.ID<<std::endl;
					}

				}
			}

			
		}*/

		std::cout << "=== functions ===" << std::endl;

		sol::state lua;
		lua.open_libraries(sol::lib::base);

		// setting a function is simple
		lua.set_function("my_add", my_add);

		// you could even use a lambda
		lua.set_function("my_mul", [](double x, double y) { return x * y; });

		// member function pointers and functors as well
		lua.set_function("mult_by_ten", multiplier{});
		lua.set_function("mult_by_five", &multiplier::by_five);

		// assert that the functions work
		lua.script("assert(my_add(10, 11) == 21)");
		lua.script("assert(my_mul(4.5, 10) == 45)");
		lua.script("assert(mult_by_ten(50) == 500)");
		lua.script("assert(mult_by_five(10) == 50)");

		// using lambdas, functions can have state.
		int x = 0;
		lua.set_function("inc", [&x]() { x += 10; });

		// calling a stateful lambda modifies the value
		lua.script("inc()");
		c_assert(x == 10);
		if (x == 10) {
			// Do something based on this information
			std::cout << "Yahoo! x is " << x << std::endl;
		}

		// this can be done as many times as you want
		lua.script(R"(
		inc()
		inc()
		inc()
		)");
		c_assert(x == 40);
		if (x == 40) {
			// Do something based on this information
			std::cout << "Yahoo! x is " << x << std::endl;
		}

		// retrieval of a function is done similarly
		// to other variables, using sol::function
		sol::function add = lua["my_add"];
		int value = add(10, 11);
		// second way to call the function
		int value2 = add.call<int>(10, 11);
		c_assert(value == 21);
		c_assert(value2 == 21);
		if (value == 21 && value2 == 21) {
			std::cout << "Woo, value is 21!" << std::endl;
		}

		std::cout << std::endl;
	}
	glm::vec2 TiledActor2D::GetScreenLocation()
	{
		return glm::vec2();
	}
	glm::mat4 TiledActor2D::GetModelMatrix()
	{
		return glm::mat4();
	}
	std::vector<float> TiledActor2D::GenerateModelSpaceVertexBuffer()
	{
		return std::vector<float>();
	}
	std::vector<Ermine::VertexAttribPointerSpecification> TiledActor2D::GetVertexArraySpecification()
	{
		return std::vector<Ermine::VertexAttribPointerSpecification>();
	}
	std::vector<uint32_t> TiledActor2D::GenerateModelSpaceIndices()
	{
		return std::vector<uint32_t>();
	}
}


