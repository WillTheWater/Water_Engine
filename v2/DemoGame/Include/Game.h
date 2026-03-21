// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/WaterEngine.h"

namespace we
{
	class Game : public WaterEngine
	{
	public:
		Game();

		void StartPlay() override;

	protected:
		// Override to create our game-specific GameInstance
		shared<GameInstance> CreateGameInstance() override;
	};
}