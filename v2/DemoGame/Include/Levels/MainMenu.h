// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once
#include "Utility/CoreMinimal.h"
#include "Framework/World/World.h"
#include "Utility/Delegate.h"

#include "Subsystem/AsyncResourceSubsystem.h"

namespace we
{
	class MainMenuUI;

	class MainMenu : public World
	{
	public:
		explicit MainMenu(EngineSubsystem& Subsystem);
		~MainMenu();

		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;
		virtual void Render() override;

	private:
		AssetHandle<texture> BgHandle;
		optional<sprite> Background;

		unique<MainMenuUI> UI;

	private:
		void OnPlayClicked();
		void ToggleSettings();
	};
}