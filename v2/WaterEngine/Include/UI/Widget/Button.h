// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"
#include "UI/Widget/Widget.h"
#include "Utility/Delegate.h"

namespace we
{
	class Button : public Widget
	{
	public:
		Button(EngineSubsystem& Subsystem, const string& Label, const string& TexturePath);

		Button(EngineSubsystem& Subsystem, const string& Label,
			const vec2f& InSize = { 150.f, 50.f },
			color FillColor = color{ 200, 200, 200 },
			color OutlineColor = color::Black,
			float OutlineThickness = 2.f);

		Delegate<> OnClicked;

		void Render(GameWindow& Window) override;
		void OnMouseEnter() override;
		void OnMouseLeave() override;
		bool OnMouseButtonPressed() override;
		void OnMouseButtonReleased() override;

	protected:
		void UpdateCache() const override;

	private:
		void UpdateVisualState();
		void PlaySound(const string& Path);

		string Label;
		shared<texture> BgTexture;
		optional<sprite> BgSprite;
		optional<rectangle> Background;
		shared<font> TextFont;
		optional<sf::Text> LabelText;

		string HoverSoundPath;
		string UnhoverSoundPath;
		string PressedSoundPath;
		string ClickSoundPath;

		color NormalColor{ 200, 200, 200 };
		color HoverColor{ 255, 255, 255 };
		color PressedColor{ 100, 100, 100 };

		bool bHovered = false;
		bool bPressed = false;
	};
}