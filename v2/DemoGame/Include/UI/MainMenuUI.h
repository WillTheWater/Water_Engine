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
	class Panel;
	class VerticalBox;
	class HorizontalBox;
	class GridBox;
	class AutoPanel;
	class Spacer;
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
		shared<Panel> MenuPanel;
		shared<Button> ColorButton;
		shared<Button> TextureButton;
	};
}
