// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
	struct EngineSubsystem;
	class AudioSubsystem;
	class WindowSubsystem;
	class RenderSubsystem;
	class TimeSubsystem;

	// =========================================================================
	// WidgetController - Base class for UI controllers
	// 
	// Mediates between UI widgets and engine subsystems.
	// UI knows nothing about subsystems - only fires delegates.
	// Controller connects delegates to subsystem calls.
	// =========================================================================
	class WidgetController
	{
	public:
		explicit WidgetController(EngineSubsystem& InSubsystem);
		virtual ~WidgetController() = default;

		// Called once after UI is created - bind all widget delegates here
		virtual void BindUI() = 0;

		// Sync UI to current subsystem values (called automatically on Show)
		virtual void SyncUIFromSettings() = 0;

		// Show the controlled UI - auto-syncs values before showing
		virtual void Show();

		// Hide the controlled UI
		virtual void Hide() = 0;

		// Check if UI is visible
		virtual bool IsVisible() const = 0;

	protected:
		// Subsystem accessors
		EngineSubsystem& GetSubsystem() const { return Subsystem; }
		AudioSubsystem& GetAudio() const;
		WindowSubsystem& GetWindow() const;
		RenderSubsystem& GetRender() const;
		TimeSubsystem& GetTime() const;

	protected:
		EngineSubsystem& Subsystem;
		bool bIsVisible = false;
	};
}
