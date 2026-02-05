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
		Button(const string& InLabel, const string& TexturePath);

		Delegate<> OnClicked;

		virtual void Update(float DeltaTime) override;
		virtual void Render(GameWindow& Window) override;
		virtual bool HandleClick(const vec2f& MousePos) override;

		virtual void OnHover() override;
		virtual void OnUnhover() override;
		virtual void OnPress() override;
		virtual void OnRelease() override;

	private:
		void UpdateVisualState();
		void UpdateSprite();

		string Label;
		shared<texture> BgTexture;
		optional<sprite> BgSprite;

		color NormalColor{ 200, 200, 200 };
		color HoverColor{ 255, 255, 255 };
		color PressedColor{ 150, 150, 150 };
		color CurrentColor{ 200, 200, 200 };

		bool bHovered = false;
		bool bPressed = false;
	};
}