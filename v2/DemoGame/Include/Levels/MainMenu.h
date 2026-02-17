// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once
#include "Core/CoreMinimal.h"
#include "Framework/World/World.h"

namespace we
{
	class MainMenuUI;

	class MainMenu : public World
	{
	public:
		explicit MainMenu(EngineSubsystem& Subsystem);
		~MainMenu();

		virtual void Construct() override;
		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;

	private:
		shared<texture> BgTexture;
		optional<sprite> BgSprite;
		unique<MainMenuUI> MenuUI;
	};
}
