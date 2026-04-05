// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include <TGUI/TGUI.hpp>

namespace we
{
	class DialogUI
	{
	public:
		DialogUI();
		~DialogUI();

		void Initialize();
		void Show();
		void Hide();
		bool IsVisible() const;

		void SetDialog(const vector<string>& Lines);
		bool Advance();
		bool HasMoreLines() const;
		size_t GetCurrentLine() const { return CurrentLine; }

		void SetPosition(const vec2f& WorldPos, const vec2f& Offset = {0.0f, 0.0f});
		void SetAnchor(const vec2f& AnchorPoint);

	private:
		void DisplayCurrentLine();

	private:
		tgui::Panel::Ptr Background;
		tgui::Label::Ptr TextLabel;
		tgui::Label::Ptr ContinueArrow;
		
		vector<string> DialogLines;
		size_t CurrentLine = 0;
		
		vec2f Anchor{0.5f, 0.5f};
		bool bInitialized = false;
		
		static constexpr float Width = 400.0f;
		static constexpr float Height = 120.0f;
	};
}