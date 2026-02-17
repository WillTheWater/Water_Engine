// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once
#include "Core/CoreMinimal.h"
#include "Utility/Delegate.h"

namespace we
{
	class Button;
	struct EngineSubsystem;

	class MainMenuUI
	{
	public:
		explicit MainMenuUI(EngineSubsystem& Subsystem);
		~MainMenuUI();

		Delegate<> OnPlayButtonClicked;
		Delegate<> OnSettingsButtonClicked;

	private:
		void CreateUI();
		void OnColorButtonClicked();
		void OnTextureButtonClicked();

		EngineSubsystem& Subsystem;
		shared<Button> ColorButton;
		shared<Button> TextureButton;
	};
}
