#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<utility>

#include "2DPrimitives/TileSet.h"

#include "EngineUI/PrecursorWindow.h"

namespace Ermine
{
	class TilesetViewer:public PrecursorWindow
	{
	public:
		TilesetViewer() = delete;
		TilesetViewer(std::filesystem::path TileSetPath);

	public:
		TilesetViewer(const TilesetViewer& rhs);
		TilesetViewer operator =(const TilesetViewer& rhs);

		TilesetViewer(TilesetViewer&& rhs);
		TilesetViewer operator =(TilesetViewer&& rhs);

		virtual void Draw() override;

	public:

	protected:

	protected:

	private:
		void HelperCalculateNumberOfRowsAndColumns();

	private:
		Ermine::TileSet Set;

		std::vector<int> Selected;
		std::pair<int, int> NumberOfRowsAndColumns;
	};
}