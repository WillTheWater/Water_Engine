// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "Utility/Delegate.h"

namespace we
{
	class GUISubsystem
	{
	public:
		GUISubsystem();

		static GUISubsystem& Get();

		void Initialize(renderTarget& ScreenUITarget, renderTarget& WorldUITarget);

		tgui::Gui& GetScreenUI() { return ScreenUI; }
		tgui::Gui& GetWorldUI() { return WorldUI; }

		void SetWindowSize(vec2u NewSize);
		void SetCameraView(const view& NewView);
		void SetCameraWorldPosition(const vec2f& WorldPos);
		
		vec2u GetWindowSize() const { return WindowSize; }
		view GetCameraView() const { return CurrentCameraView; }
		
		// World-space UI positioning
		void SetWidgetWorldPosition(tgui::Widget::Ptr Widget, const vec2f& WorldPos);
		void SyncWorldPositions();
		
		bool HandleEvent(const event& Event);

		void RequestFullscreen(bool bEnabled);
		Delegate<bool> OnFullscreenRequested;

	private:
		static GUISubsystem* Instance;

		tgui::Gui ScreenUI;
		tgui::Gui WorldUI;
		
		vec2u WindowSize;
		view CurrentCameraView;
		vec2f CameraWorldPosition;
		
		// Track WorldUI widgets in world coordinates
		dictionary<tgui::Widget::Ptr, vec2f> WorldPositions;
	};

	inline GUISubsystem& MakeGUI() { return GUISubsystem::Get(); }
}