// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/WidgetController.h"
#include "Framework/EngineSubsystem.h"
#include "Subsystem/AudioSubsystem.h"
#include "Subsystem/WindowSubsystem.h"
#include "Subsystem/RenderSubsystem.h"
#include "Subsystem/TimeSubsystem.h"

namespace we
{
	WidgetController::WidgetController(EngineSubsystem& InSubsystem)
		: Subsystem(InSubsystem)
	{
	}

	void WidgetController::Show()
	{
		if (bIsVisible) return;
		bIsVisible = true;
		SyncUIFromSettings();
	}

	AudioSubsystem& WidgetController::GetAudio() const
	{
		return *Subsystem.Audio;
	}

	WindowSubsystem& WidgetController::GetWindow() const
	{
		return *Subsystem.Window;
	}

	RenderSubsystem& WidgetController::GetRender() const
	{
		return *Subsystem.Render;
	}

	TimeSubsystem& WidgetController::GetTime() const
	{
		return *Subsystem.Time;
	}
}
