// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/World.h"

namespace we
{
	class PauseUI;
	class Player;
	class Camera;
	class NPC;
	class Aoi;
	class Kiyoshi;

	class LevelOne : public World
	{
	public:
		explicit LevelOne(EngineSubsystem& Subsystem);
		~LevelOne();

		virtual void Construct() override;
		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;

	private:
		void SetupInputBindings();
		void OnTogglePause();
		void OnPauseContinue();
		void OnPauseSettings();
		void OnPauseQuit();
		void OnSettingsClosed();

		void ShowPauseMenu();
		void HidePauseMenu();

	private:
		shared<texture> BgTexture;
		optional<sprite> Background;
		unique<PauseUI> PauseMenuUI;
		bool bPauseMenuOpen = false;
		bool bSettingsOpen = false;

		// NPCs
		std::weak_ptr<Aoi> AoiRef;
		std::weak_ptr<Kiyoshi> KiyoshiRef;
		
		// Player and Camera
		std::weak_ptr<Player> PlayerRef;
		std::weak_ptr<Camera> CameraRef;
		sf::VertexArray OriginMarker;
	};
}
