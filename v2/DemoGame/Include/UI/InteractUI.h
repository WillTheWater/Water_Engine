// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include <TGUI/TGUI.hpp>

namespace we
{
	// Semi-transparent interaction prompt widget
	// Attach to any actor to show "E to Interact!" above their head
	class InteractUI
	{
	public:
		InteractUI();
		~InteractUI();

		// Create the widget once (call in BeginPlay)
		void Initialize(const string& Text = "E to Interact!");

		// Show existing widget
		void Show();

		// Hide existing widget
		void Hide();

		// Update position in world space
		void SetPosition(const vec2f& WorldPos);

		// Set custom prompt text
		void SetText(const string& Text);

		// Check if currently visible
		bool IsVisible() const { return bVisible; }

	private:
		tgui::Panel::Ptr Background;
		tgui::Label::Ptr TextLabel;
		bool bVisible = false;
		bool bInitialized = false;
		static constexpr float Width = 120.0f;
		static constexpr float Height = 30.0f;
	};
}
