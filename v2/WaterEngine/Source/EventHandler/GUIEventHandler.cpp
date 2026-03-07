// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "EventHandler/GUIEventHandler.h"
#include "Subsystem/GuiSubsystem.h"

namespace we
{
	void GUIEventHandler::operator()(const event::MouseButtonPressed& e)
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

	void GUIEventHandler::operator()(const event::MouseButtonReleased& e)
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

	void GUIEventHandler::operator()(const event::MouseMoved& e)
	{
		GUI.GetScreenUI().handleEvent(e);
		GUI.GetWorldUI().handleEvent(e);
		
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

	void GUIEventHandler::operator()(const event::MouseWheelScrolled& e)
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