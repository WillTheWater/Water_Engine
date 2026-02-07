// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"
#include <Framework/WaterEngine.h>
#include "UI/PauseUI.h"

namespace we
{
	class Game : public WaterEngine
	{
	public:
		Game();

		virtual void Tick(float DeltaTime) override;
		void TogglePause();
	private:
		void OnStateEnter(shared<IGameStateToken> NewState);
		void OnStateExit(shared<IGameStateToken> OldState);
		void BindInput();
		unique<PauseUI> PauseMenu;
		bool bPaused;
	};
}