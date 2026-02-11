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
		explicit Panel(EngineSubsystem& Subsystem, const string& BackgroundTexturePath = "");

		void AddChild(shared<Widget> Child);
		void RemoveChild(Widget* Child);
		void ClearChildren();

		void Update(float DeltaTime) override;
		void Render(GameWindow& Window) override;

		void OnMouseEnter() override;
		void OnMouseLeave() override;
		void OnMouseMoved(const vec2f& MousePos) override;
		bool OnMouseButtonPressed() override;
		void OnMouseButtonReleased() override;

	private:
		list<weak<Widget>> Children;
		weak<Widget> HoveredChild;
		shared<texture> BgTexture;
		optional<sprite> BgSprite;

		shared<Widget> FindChildAt(const vec2f& WorldPoint) const;
	};
}