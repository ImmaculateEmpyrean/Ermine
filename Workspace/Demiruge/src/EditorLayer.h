#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "Ermine.h"

namespace Editor
{
	class EditorLayer : public Ermine::LayerStackLayer
	{
	public:
		EditorLayer();

	public:
		virtual void Update(float Dt) override; //The Update Function Is Called Generally At 25fps To Update The Engine..
		virtual void Render(float Dt) override; //This Function Runs As Fast As possible Depending On the Machine..
	};
}