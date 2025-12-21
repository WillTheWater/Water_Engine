#include "UI/HUD.h"

namespace we
{
	HUD::HUD()
		: bIsInitialized{false}
	{
	}

	void HUD::NativeInitialize(Renderer& GameRenderer)
	{
		if (!bIsInitialized)
		{
			bIsInitialized = true;
			Initialize(GameRenderer);
		}
	}
	
	void HUD::Initialize(Renderer& GameRenderer)
	{
	}
	
	bool HUD::HandleEvent(const optional<sf::Event> Event)
	{
		return false;
	}
}