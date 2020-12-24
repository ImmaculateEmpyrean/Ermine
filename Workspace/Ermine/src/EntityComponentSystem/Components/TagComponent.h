#pragma once
#include<iostream>
#include<vector>
#include<string>

/*
* The tag Components Generally Holds The Name Of The Entity In The Entity Component System..
*/
namespace Ermine
{
	struct TagComponent
	{
	public:
		TagComponent() = default;
		TagComponent(std::string Tag)
			:
			Tag(Tag)
		{}

		std::string Tag = "NoNameTag";
	};
}