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

		Delegate<> OnClicked;

		virtual void Update(float DeltaTime) override;
		virtual void Render(GameWindow& Window) override;

	public:
		string HoverSoundPath;
		string UnhoverSoundPath;
		string PressedSoundPath;
		string ClickSoundPath;

	private:
		void UpdateVisualState();
		void PlayHoverSound();
		void PlayUnhoverSound();
		void PlayPressedSound();
		void PlayClickSound();

	private:
		string Label;
		shared<texture> BgTexture;
		optional<sprite> BgSprite;

		shared<font> TextFont;
		optional<sf::Text> LabelText;

		color NormalColor{ 200, 200, 200 };
		color HoverColor{ 255, 255, 255 };
		color PressedColor{ 150, 150, 150 };
		color CurrentColor{ 200, 200, 200 };

		bool bHovered = false;
		bool bPressed = false;
	};
}