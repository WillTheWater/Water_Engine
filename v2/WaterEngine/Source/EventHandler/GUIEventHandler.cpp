// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "EventHandler/GUIEventHandler.h"
#include "Subsystem/GuiSubsystem.h"

namespace we
{
	void GUIEventHandler::operator()(const sf::Event::MouseButtonPressed& e)
	{
		// handleEvent returns true if a widget (Button, etc.) handled the click
		// Clicking empty space returns false - game gets the event
		
		if (GUI.GetScreenUI().handleEvent(e))
		{
			Consumed = true;  // Clicked on ScreenUI button/widget
			return;
		}
		
		if (GUI.GetWorldUI().handleEvent(e))
		{
			Consumed = true;  // Clicked on WorldUI button/widget
			return;
		}
		
		Consumed = false;  // Clicked on empty space - pass to game
	}

	void GUIEventHandler::operator()(const sf::Event::MouseButtonReleased& e)
	{
		// Only consume if a widget handled the release (e.g., button click completed)
		
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

	void GUIEventHandler::operator()(const sf::Event::MouseMoved& e)
	{
		// Always update TGUI hover states, but don't consume
		// Game needs mouse position for aiming/cursor
		GUI.GetScreenUI().handleEvent(e);
		GUI.GetWorldUI().handleEvent(e);
		
		Consumed = false;
	}

	void GUIEventHandler::operator()(const sf::Event::KeyPressed& e)
	{
		// Try ScreenUI (ESC on menu, text input, etc.)
		if (GUI.GetScreenUI().handleEvent(e))
		{
			Consumed = true;
			return;
		}
		
		// Try WorldUI
		if (GUI.GetWorldUI().handleEvent(e))
		{
			Consumed = true;
			return;
		}
		
		Consumed = false;
	}

	void GUIEventHandler::operator()(const sf::Event::KeyReleased& e)
	{
		// Try ScreenUI
		if (GUI.GetScreenUI().handleEvent(e))
		{
			Consumed = true;
			return;
		}
		
		// Try WorldUI
		if (GUI.GetWorldUI().handleEvent(e))
		{
			Consumed = true;
			return;
		}
		
		Consumed = false;
	}

	void GUIEventHandler::operator()(const sf::Event::MouseWheelScrolled& e)
	{
		// Try ScreenUI (scroll panels, etc.)
		if (GUI.GetScreenUI().handleEvent(e))
		{
			Consumed = true;
			return;
		}
		
		// Try WorldUI
		if (GUI.GetWorldUI().handleEvent(e))
		{
			Consumed = true;
			return;
		}
		
		Consumed = false;
	}
}
