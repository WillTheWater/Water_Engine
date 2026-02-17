// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include <Framework/WaterEngine.h>
#include "UI/PauseUI.h"

namespace we
{
	class Game : public WaterEngine
	{
	public:
		Game();

	public:
		void TogglePause();

	protected:
		virtual void Construct() override;
		virtual void BeginPlay() override;
		virtual void Tick(float) override;

	private:
		//unique<PauseUI> PauseMenu;
		bool bPaused;

	private:
		void RegisterLevels();
		void OnStateEnter(shared<IGameStateToken> NewState);
		void OnStateExit(shared<IGameStateToken> OldState);
		void BindInput();
	};
}