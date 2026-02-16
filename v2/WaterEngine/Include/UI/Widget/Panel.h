//// =============================================================================
//// Water Engine v2.0.0
//// Copyright(C) 2026 Will The Water
//// =============================================================================
//
//#pragma once
//
//#include "Core/CoreMinimal.h"
//#include "UI/Widget/Widget.h"
//
//namespace we
//{
//	class Panel : public Widget
//	{
//	public:
//		Panel(EngineSubsystem& Subsystem,
//			const vector<shared<Widget>>& InChildren,
//			rectf Margins = {{0.f,0.f},{0.f,0.f}},
//			Anchor InTargetAnchor = Anchor::Center,
//			Anchor InWidgetAnchor = Anchor::Center,
//			const vec2f& InOffset = { 0.f, 0.f });
//
//		Panel(EngineSubsystem& Subsystem, const string& TexturePath);
//
//		void Update(float DeltaTime) override;
//		void Render(GameWindow& Window) override;
//
//	private:
//		optional<rectangle> Background;
//	};
//}