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
		shared<texture> BgTexture;
		optional<sprite> Background;
		unique<MainMenuUI> UI;

		// Original tests
		AssetHandle<texture> TestHandle;
		optional<sprite> TestSpriteTick;
		AssetHandle<texture> TestHandleRender;
		optional<sprite> TestSpriteRender;
		shared<texture> TestTextureSync;
		optional<sprite> TestSpriteSync;

		// Power tests
		list<AssetHandle<texture>> MassHandles;
		bool bMassTestLogged = false;
		float MassTestStartTime = 0.0f;

		list<AssetHandle<texture>> PriorityHandles;
		bool bPriorityTestLogged = false;

	private:
		void OnPlayClicked();
	};
}