#pragma once
#include<iostream>
#include<vector>
#include<string>

namespace Ermine
{
	class App;

	class DeltaTime
	{
	public:
		DeltaTime() = default;

	public:
		//Get Delta Time Api..
		double GetSeconds();
		double GetMilliSeconds();

		//This Operator Returns The Delta Time In Seconds..
		operator double();

	public:

	protected:

	protected:

	private:
		void Update();

	private:
		double DTime = 0.0f;
		double Time  = 0.0f;

		//Since This Variable Is Pretty Much Accessed From Many Threads.. Better Lock It Just To Be Sure..
		std::recursive_mutex Mut;

		friend class Ermine::App;
	};
}