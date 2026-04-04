// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Subsystem/InputSubsystem.h"
#include "Subsystem/CursorSubsystem.h"
#include "Subsystem/GuiSubsystem.h"
#include <TGUI/Widgets/Button.hpp>

namespace we
{
	// Utility for styling buttons consistently
	namespace UIHelper
	{
		inline void StyleControllerButton(tgui::Button::Ptr Button)
		{
			auto Renderer = Button->getRenderer();
			Renderer->setBackgroundColor(tgui::Color{ 0, 16, 31 });
			Renderer->setBackgroundColorHover(tgui::Color{ 47, 121, 142 });
			Renderer->setBackgroundColorFocused(tgui::Color{ 47, 121, 142 });
			Renderer->setBackgroundColorDown(tgui::Color{ 133, 120, 81 });
			Renderer->setTextColor(tgui::Color::White);
			Renderer->setTextColorHover(tgui::Color::White);
			Renderer->setTextColorFocused(tgui::Color::White);
			Renderer->setTextColorDown(tgui::Color::White);
			Renderer->setBorderColor(tgui::Color::White);
			Renderer->setBorderColorHover(tgui::Color::White);
			Renderer->setBorderColorFocused(tgui::Color::White);
			Renderer->setBorders(tgui::Outline(2));
			Button->setFocusable(true);
		}
	}

	// Interface for UIs that support controller navigation
	class IControllerUI
	{
	public:
		virtual ~IControllerUI() = default;

		// Call this in Show() after setting visible
		void HandleControllerShow()
		{
			if (InputController().IsControllerConnected(0))
			{
				GetCursor().SetVisibility(false);
				SetInitialFocus();
			}
			else
			{
				GetCursor().SetVisibility(true);
			}
		}

		// Call this when controller connects while UI is visible
		void OnControllerConnected(int GamepadID)
		{
			if (!IsUIFocused() || GamepadID != 0)
				return;

			GetCursor().SetVisibility(false);
			SetInitialFocus();
		}

		// Call this when controller disconnects while UI is visible
		void OnControllerDisconnected(int GamepadID)
		{
			if (!IsUIFocused() || GamepadID != 0)
				return;

			GetCursor().SetVisibility(true);

			auto& GUI = MakeGUI().GetScreenUI();
			if (auto Focused = GUI.getFocusedLeaf())
				Focused->setFocused(false);
		}

	protected:
		// Override this to set focus on your first widget
		virtual void SetInitialFocus() = 0;

		// Override this to check if this UI is currently active/focused
		virtual bool IsUIFocused() const = 0;
	};
}
