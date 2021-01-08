#pragma once
#include <string>

class MD5Hasher
{
public:
	std::string CalculateStringHash(const std::string& input);

	inline std::string GetName() const
	{
		return "MD5";
	}
};