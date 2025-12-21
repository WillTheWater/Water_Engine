#include "UI/HUD.h"

namespace we
{
	HUD::HUD()
		: bIsInitialized{false}
	{
	}

	

	void HUD::NativeInitialize(const sf::RenderWindow& Window)
	{
		if (!bIsInitialized)
		{
			bIsInitialized = true;
			Initialize(Window);
		}
	}
	
	void HUD::Initialize(const sf::RenderWindow& Window)
	{
	}
	
	bool HUD::HandleEvent(const optional<sf::Event> Event)
	{
		return false;
	}
}