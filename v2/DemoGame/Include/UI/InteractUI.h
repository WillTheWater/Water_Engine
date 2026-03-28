// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include <TGUI/TGUI.hpp>

namespace we
{
	class InteractUI
	{
	public:
		InteractUI();
		~InteractUI();

		void Initialize(const string& Text = "E to Interact!");

		void Show();
		void Hide();

		void SetPosition(const vec2f& WorldPos);
		void SetText(const string& Text);
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