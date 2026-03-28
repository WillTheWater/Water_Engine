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
		bool IsVisible() const;

		void SetPosition(const vec2f& WorldPos, const vec2f& Offset = {0.0f, 0.0f});
		void SetText(const string& Text);
		void SetAnchor(const vec2f& AnchorPoint);

	private:
		tgui::Panel::Ptr Background;
		tgui::Label::Ptr TextLabel;
		vec2f Anchor{0.5f, 1.0f};
		bool bInitialized = false;
		static constexpr float Width = 120.0f;
		static constexpr float Height = 30.0f;
	};
}