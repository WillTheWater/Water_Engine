#pragma once

#include<string>

std::string GetAssetDirectory()
{
#ifdef  NDEBUG
	return "assets/";
#else
	return "C:/Dev/Water_Engine/TemplateGame/assets/";
#endif
}
