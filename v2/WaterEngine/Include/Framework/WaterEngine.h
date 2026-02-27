// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Core/EngineConfig.h"
#include "Framework/EngineSubsystem.h"

namespace we
{
	class Editor;
	enum class EngineMode { Editor, Play };

	class WaterEngine
	{
	public:
		WaterEngine();
		virtual ~WaterEngine();

		// Main loop functions
		void Initialize();
		void Tick();
		void Render();
		void ProcessEvents();
		bool IsRunning() const;

		// Mode control
		void SetMode(EngineMode NewMode);
		bool IsInEditor() const { return CurrentMode == EngineMode::Editor; }
		bool IsPlaying() const { return CurrentMode == EngineMode::Play; }

	protected:
		EngineSubsystem Subsystem;
		unique<Editor> EditorInstance;
		EngineMode CurrentMode = EngineMode::Editor;

		virtual void TickGame(float DeltaTime);
		virtual void TickEditor(float DeltaTime);

	private:
		void PreConstruct();
		void MountAssetDirectory();
		void CreateSubsystems();
		void LoadEngineConfig();
	};
}
