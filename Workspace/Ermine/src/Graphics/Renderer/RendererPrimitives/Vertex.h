#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<memory>

#include "VertexLayout.h"
#include "VertexDataObject.h"
#include "EVertexDataObjectType.h"

#include <type_traits>

namespace Ermine
{
	class Vertex
	{
	public:
		Vertex() = delete;
		Vertex(std::shared_ptr<VertexLayout> Layout);

	public:
		bool IsLayoutSame(const Vertex& rhs);
		std::size_t GetVertexSize();

		template<typename T>
		void AppendValue(T Value) 
		{
			int Location = ;

			//auto StrongLayout = Layout.lock();
			auto Type = Layout->GetElementType(Buffer.size());

			if (std::is_same<T, float>::value)
			{
				if (Type == Ermine::EVertexDataObjectType::FLOAT)
				{
					//ReadyState = false;
					Buffer.emplace_back(VertexDataObject(Value));
					return;
				}
				else
				{
					STDOUTDefaultLog_Critical("The Value Type Cannot Be Added As Per Layout Specification ERROR..  File : {} Function {} Line : {}",__FILE__,__FUNCTION__,__LINE__);
					__debugbreak();
				}
			}

			else if (std::is_same<T, int8_t>::value)
			{
				if (Type == Ermine::EVertexDataObjectType::BYTE)
				{
					ReadyState = false;
					Buffer.emplace_back(VertexDataObject(Value));
					return;
				}
				else
				{
					STDOUTDefaultLog_Critical("The Value Type Cannot Be Added As Per Layout Specification ERROR..  File : {} Function {} Line : {}", __FILE__, __FUNCTION__, __LINE__);
					__debugbreak();
				}
			}
			else if (std::is_same<T, uint8_t>::value)
			{
				if (Type == Ermine::EVertexDataObjectType::UBYTE)
				{
					ReadyState = false;
					Buffer.emplace_back(VertexDataObject(Value));
					return;
				}
				else
				{
					STDOUTDefaultLog_Critical("The Value Type Cannot Be Added As Per Layout Specification ERROR..  File : {} Function {} Line : {}", __FILE__, __FUNCTION__, __LINE__);
					__debugbreak();
				}
			}

			else if (std::is_same<T, int>::value)
			{
				if (Type == Ermine::EVertexDataObjectType::INT)
				{
					ReadyState = false;
					Buffer.emplace_back(VertexDataObject(Value));
					return;
				}
				else
				{
					STDOUTDefaultLog_Critical("The Value Type Cannot Be Added As Per Layout Specification ERROR..  File : {} Function {} Line : {}", __FILE__, __FUNCTION__, __LINE__);
					__debugbreak();
				}
			}
			else if (std::is_same<T, unsigned int>::value)
			{
				if (Type == Ermine::EVertexDataObjectType::UINT)
				{
					ReadyState = false;
					Buffer.emplace_back(VertexDataObject(Value));
					return;
				}
				else
				{
					STDOUTDefaultLog_Critical("The Value Type Cannot Be Added As Per Layout Specification ERROR..  File : {} Function {} Line : {}", __FILE__, __FUNCTION__, __LINE__);
					__debugbreak();
				}
			}
			else {
				STDOUTDefaultLog_Critical("Adding This Type Of Value Is Not Yet Supported By This Function As Of Now File : {} Function {} Line : {}", __FILE__, __FUNCTION__, __LINE__);
			}
		}
		template<typename T>
		void AppendValues(std::vector<T> ValueVector)
		{
			for (auto i : ValueVector)
				AppendValue(i);
		}
		void SetVertex(std::vector<VertexDataObject> Vertex) { Buffer = Vertex; } //This Must Be A Full Vertex Or It Will Throw An Error..
		void Set(int Location, int ElementIndex, VertexDataObject Obj);
		void Erase(int Location, int ElementIndex);

		std::pair<std::unique_ptr<uint8_t>, std::size_t> GetVertex();
		std::weak_ptr<VertexLayout> GetLayout() { return Layout; }

		void Clear();
	private:
		std::shared_ptr<VertexLayout> Layout;
		std::vector<VertexDataObject> Buffer;
	};
}