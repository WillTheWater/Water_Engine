// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/GuiSubsystem.h"
#include "EventHandler/GUIEventHandler.h"
#include "Utility/Log.h"

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

	event GUISubsystem::TransformEvent(const event& event) const
	{
		// ======================================================
		//			THIS ALL NEEDS TO BE REFACTORED
		// ======================================================
		
		
		// Transform window coordinates to render target (1920x1080) coordinates
		auto TransformPoint = [&](int x, int y) -> sf::Vector2i {
			// Step 1: Normalize window coordinates (0-1 range accounting for viewport)
			sf::FloatRect viewport = CurrentCameraView.getViewport();
			
			// Calculate normalized position within the viewport
			float normX = (static_cast<float>(x) / WindowSize.x - viewport.position.x) / viewport.size.x;
			float normY = (static_cast<float>(y) / WindowSize.y - viewport.position.y) / viewport.size.y;
			
			// Step 2: Scale to 1920x1080 render target
			return sf::Vector2i(
				static_cast<int>(normX * 1920.0f),
				static_cast<int>(normY * 1080.0f)
			);
		};
		
		if (auto* mouse = event.getIf<event::MouseMoved>()) {
			sf::Vector2i newPos = TransformPoint(mouse->position.x, mouse->position.y);
			return event::MouseMoved{newPos};
		}
		
		if (auto* mouse = event.getIf<event::MouseButtonPressed>()) {
			sf::Vector2i newPos = TransformPoint(mouse->position.x, mouse->position.y);
			return event::MouseButtonPressed{mouse->button, newPos};
		}
		
		if (auto* mouse = event.getIf<event::MouseButtonReleased>()) {
			sf::Vector2i newPos = TransformPoint(mouse->position.x, mouse->position.y);
			return event::MouseButtonReleased{mouse->button, newPos};
		}
		
		if (auto* wheel = event.getIf<event::MouseWheelScrolled>()) {
			sf::Vector2i newPos = TransformPoint(wheel->position.x, wheel->position.y);
			return event::MouseWheelScrolled{wheel->wheel, wheel->delta, newPos};
		}
		
		// Non-mouse events pass through unchanged
		return event;
	}

	bool GUISubsystem::HandleEvent(const event& Event)
	{
		// Transform window coordinates to render target coordinates
		event transformedEvent = TransformEvent(Event);
		
		GUIEventHandler Handler{*this};
		transformedEvent.visit(Handler);
		return Handler.Consumed;
	}
}
