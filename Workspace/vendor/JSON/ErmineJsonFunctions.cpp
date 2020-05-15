#include "ErmineJsonFunctions.h"
#include<vector>

namespace Ermine
{
	std::vector<int> ExtractIntDataFromJsonArray(std::string ArrayString)
	{
		ArrayString.erase(std::remove(ArrayString.begin(), ArrayString.end(), '['), ArrayString.end());
		ArrayString.erase(std::remove(ArrayString.begin(), ArrayString.end(), ']'), ArrayString.end());

		std::string Line;
		std::vector<int> ElementsArray;
		for (auto ch : ArrayString)
		{
			if (ch != ',')
				Line.push_back(ch);
			else
			{
				ElementsArray.emplace_back(std::stoi(Line));
				Line.clear();
			}
		}
		ElementsArray.emplace_back(std::stof(Line)); //This Is For THe LAst Coordinate Not Followed By a ,
		Line.clear();

		return ElementsArray;
	}
}