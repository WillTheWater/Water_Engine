// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/World.h"

namespace we
{
	class MainMenu : public World
	{
	public:
		MainMenu(EngineSubsystem& Subsystem);

		void BeginPlayGlobal();
		void TickGlobal(float DeltaTime);

	private:
		weak<Actor> Background;
		weak<Actor> TitleCard;
	};
}
