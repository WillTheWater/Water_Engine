// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
	class MainMenuUI
	{
	public:
		MainMenuUI();
		
		void Initialize();
		void Show();
		void Hide();
		
	private:
		void SetupLayout();
		void OnPlayClicked();
		void OnSettingsClicked();
		void OnQuitClicked();
		
		bool bVisible = false;
		bool bInitialized = false;
	};
}