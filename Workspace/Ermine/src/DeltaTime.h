#pragma once
#include<iostream>
#include<vector>
#include<string>

namespace Ermine
{
	class DeltaTime
	{
	public:
		DeltaTime(double Time = 0.0);

	public:
		double GetSeconds();
		double GetMilliSeconds();

		//This Operator Returns The Delta Time In Seconds..
		operator double();

	public:

	protected:

	protected:

	private:

	private:
		//this is an internal variable which stores time in seconds..
		double Time; 

	};
}