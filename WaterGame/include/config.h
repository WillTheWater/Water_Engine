#pragma once

#include<string>

std::string GetAssetDirectory()
{
#ifdef  NDEBUG
	return "assets/";
#else
	return "C:/Dev/WaterEngine/WaterGame/assets/";
#endif
}
