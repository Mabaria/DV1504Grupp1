#pragma once

//#ifndef CorrectName_H
//#define CorrectName_H

#include <string>

namespace Name
{
	static std::string CorrectName(std::string name)
	{
		std::string newName = "";

		for (int i = 0; i < (int)name.size(); i++)
		{
			int c = name[i];

			switch (name[i])
			{
			case -91: // ¥
				newName += '\xE5'; // 'å'
				break;
			case -92: // ¤
				newName += '\xE4'; // 'ä'
				break;
			case -74: // ¶
				newName += '\xF6'; // 'ö'
				break;

			case -61: // Character skip
				break;

			default:
				newName += name[i];
				break;
			}
		}
		return newName;
	}

	static std::string GetTabs(int stringSize)
	{
		if (stringSize < 4)
			return "\t\t\t\t\t\t\t\t\t";
		if (stringSize < 8)
			return "\t\t\t\t\t\t\t\t";
		if (stringSize < 12)
			return "\t\t\t\t\t\t\t";
		if (stringSize < 16)
			return "\t\t\t\t\t\t";
		if (stringSize < 20)
			return "\t\t\t\t\t";
		if (stringSize < 24)
			return "\t\t\t\t";
		if (stringSize < 28)
			return "\t\t\t";
		if (stringSize < 32)
			return "\t\t";
		else
			return "\t";
	}
};
//#endif // !1

