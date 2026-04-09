// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "EventHandler/GUIEventHandler.h"
#include "Subsystem/GuiSubsystem.h"

namespace we
{
	vec2i GUIEventHandler::TransformMousePos(int x, int y) const
	{
		vec2u WindowSize = GUI.GetWindowSize();
		view CameraView = GUI.GetCameraView();
		
		rectf viewport = CameraView.getViewport();
		
		float normX = (static_cast<float>(x) / WindowSize.x - viewport.position.x) / viewport.size.x;
		float normY = (static_cast<float>(y) / WindowSize.y - viewport.position.y) / viewport.size.y;
		
		return vec2i(static_cast<int>(normX * 1920.0f),	static_cast<int>(normY * 1080.0f));
	}

	void GUIEventHandler::operator()(const event::MouseButtonPressed& e)
	{
		vec2i newPos = TransformMousePos(e.position.x, e.position.y);
		event::MouseButtonPressed transformed{e.button, newPos};
		
		if (GUI.GetScreenUI().handleEvent(transformed))
		{
			Consumed = true;
			return;
		}
		
		if (GUI.GetWorldUI().handleEvent(transformed))
		{
			Consumed = true;
			return;
		}
		
		Consumed = false;
	}

	void GUIEventHandler::operator()(const event::MouseButtonReleased& e)
	{
		vec2i newPos = TransformMousePos(e.position.x, e.position.y);
		event::MouseButtonReleased transformed{e.button, newPos};
		
		if (GUI.GetScreenUI().handleEvent(transformed))
		{
			Consumed = true;
			return;
		}
		
		if (GUI.GetWorldUI().handleEvent(transformed))
		{
			Consumed = true;
			return;
		}
		
		Consumed = false;
	}

	void GUIEventHandler::operator()(const event::MouseMoved& e)
	{
		vec2i newPos = TransformMousePos(e.position.x, e.position.y);
		event::MouseMoved transformed{newPos};
		
		GUI.GetScreenUI().handleEvent(transformed);
		GUI.GetWorldUI().handleEvent(transformed);
		
		Consumed = false;
	}

	void GUIEventHandler::operator()(const event::MouseWheelScrolled& e)
	{
		vec2i newPos = TransformMousePos(e.position.x, e.position.y);
		event::MouseWheelScrolled transformed{e.wheel, e.delta, newPos};
		
		if (GUI.GetScreenUI().handleEvent(transformed))
		{
			Consumed = true;
			return;
		}
		
		if (GUI.GetWorldUI().handleEvent(transformed))
		{
			Consumed = true;
			return;
		}
		
		Consumed = false;
	}

	void GUIEventHandler::operator()(const event::KeyPressed& e)
	{
		if (GUI.GetScreenUI().handleEvent(e))
		{
			Consumed = true;
			return;
		}
		
		if (GUI.GetWorldUI().handleEvent(e))
		{
			Consumed = true;
			return;
		}
		
		Consumed = false;
	}

	void GUIEventHandler::operator()(const event::KeyReleased& e)
	{
		if (GUI.GetScreenUI().handleEvent(e))
		{
			Consumed = true;
			return;
		}
		
		if (GUI.GetWorldUI().handleEvent(e))
		{
			Consumed = true;
			return;
		}
		
		Consumed = false;
	}

	void GUIEventHandler::operator()(const event::TextEntered& e)
	{
		if (GUI.GetScreenUI().handleEvent(e))
		{
			Consumed = true;
			return;
		}

		if (GUI.GetWorldUI().handleEvent(e))
		{
			Consumed = true;
			return;
		}

		Consumed = false;
	}
}