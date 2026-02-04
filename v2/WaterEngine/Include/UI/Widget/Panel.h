// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once
#include "Utility/CoreMinimal.h"
#include "UI/Widget/Widget.h"

namespace we
{
	class Panel : public Widget
	{
	public:
		Panel(const string& BackgroundTexturePath = "");

		void AddChild(shared<Widget> Child);
		void RemoveChild(Widget* Child);
		void ClearChildren();

		virtual void Update(float DeltaTime) override;
		virtual void Render(GameWindow& Window) override;
		virtual bool HandleClick(const vec2f& MousePos) override;

	private:
		list<shared<Widget>> Children;
		shared<texture> BgTexture;
		optional<sprite> BgSprite;
	};
}