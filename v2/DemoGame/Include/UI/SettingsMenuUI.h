// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once
#include "Utility/CoreMinimal.h"
#include "Utility/Delegate.h"

namespace we
{
	class Button;
	class Panel;
	struct EngineSubsystem;

	class SettingsMenuUI
	{
	public:
		SettingsMenuUI(EngineSubsystem& Subsystem);

	public:
		Delegate<>OnExitClicked;

	private:
		void CreateUI();
		void OnExitButtonClicked();

		EngineSubsystem& Subsystem;
		shared<Button> ExitButton;
	};
}