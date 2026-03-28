// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/GuiSubsystem.h"
#include "EventHandler/GUIEventHandler.h"

namespace we
{
	GUISubsystem* GUISubsystem::Instance = nullptr;

	GUISubsystem::GUISubsystem()
		: WindowSize{1920, 1080}
		, CurrentCameraView{}
	{
		Instance = this;
	}

	GUISubsystem& GUISubsystem::Get()
	{
		return *Instance;
	}

	void GUISubsystem::Initialize(renderTarget& ScreenUITarget, renderTarget& WorldUITarget)
	{
		ScreenUI.setTarget(ScreenUITarget);
		WorldUI.setTarget(WorldUITarget);
	}

	void GUISubsystem::SetWindowSize(vec2u NewSize)
	{
		WindowSize = NewSize;
	}

	void GUISubsystem::SetCameraView(const view& NewView)
	{
		CurrentCameraView = NewView;
	}

	void GUISubsystem::SetCameraWorldPosition(const vec2f& WorldPos)
	{
		CameraWorldPosition = WorldPos;
	}

	void GUISubsystem::SetWidgetWorldPosition(tgui::Widget::Ptr Widget, const vec2f& WorldPos)
	{
		WorldPositions[Widget] = WorldPos;
	}

	void GUISubsystem::SyncWorldPositions()
	{
		vec2f ViewSize = vec2f{CurrentCameraView.getSize().x, CurrentCameraView.getSize().y};
		vec2f CamTopLeft = CameraWorldPosition - ViewSize / 2.0f;
		
		float Padding = 150.0f;
		vec2f BoundsMin = CamTopLeft - vec2f{Padding, Padding};
		vec2f BoundsMax = CamTopLeft + ViewSize + vec2f{Padding, Padding};

		for (auto& [Widget, WorldPos] : WorldPositions)
		{
			if (!Widget->isVisible())
				continue;
			
			if (WorldPos.x < BoundsMin.x || WorldPos.x > BoundsMax.x ||
			    WorldPos.y < BoundsMin.y || WorldPos.y > BoundsMax.y)
			{
				continue;
			}

			vec2f ScreenPos = WorldPos - CamTopLeft;
			Widget->setPosition({ScreenPos.x, ScreenPos.y});
		}
	}

	bool GUISubsystem::HandleEvent(const event& Event)
	{
		GUIEventHandler Handler{*this};
		Event.visit(Handler);
		return Handler.Consumed;
	}

	void GUISubsystem::RequestFullscreen(bool bEnabled)
	{
		OnFullscreenRequested.Broadcast(bEnabled);
	}
}