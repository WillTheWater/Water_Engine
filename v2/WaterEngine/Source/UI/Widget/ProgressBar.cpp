//// =============================================================================
//// Water Engine v2.0.0
//// Copyright(C) 2026 Will The Water
//// =============================================================================
//
//#include "UI/Widget/ProgressBar.h"
//#include "Subsystem/WindowSubsystem.h"
//
//namespace we
//{
//	ProgressBar::ProgressBar(EngineSubsystem& Subsystem,
//		const vec2f& InSize,
//		float InitialValue)
//		: Widget(Subsystem)
//	{
//		SetSize(InSize);
//		SetValue(InitialValue);
//
//		// Background rectangle
//		Background.emplace();
//		Background->setSize(InSize);
//		Background->setFillColor({ 50, 50, 50 });
//
//		// Fill rectangle
//		Fill.emplace();
//		Fill->setFillColor({ 100, 200, 100 });
//	}
//
//	void ProgressBar::SetValue(float NewValue)
//	{
//		Value = std::clamp(NewValue, 0.f, 1.f);
//	}
//
//	void ProgressBar::SetDirection(ProgressDirection InDirection)
//	{
//		Direction = InDirection;
//	}
//
//	void ProgressBar::SetBackgroundColor(color Color)
//	{
//		if (Background)
//			Background->setFillColor(Color);
//	}
//
//	void ProgressBar::SetFillColor(color Color)
//	{
//		if (Fill)
//			Fill->setFillColor(Color);
//	}
//
//	void ProgressBar::Update(float DeltaTime)
//	{
//		
//	}
//
//	void ProgressBar::Render(GameWindow& Window)
//	{
//		if (!IsVisible()) return;
//
//		vec2f Pos = GetWorldPosition();
//		vec2f Size = GetSize();
//
//		if (Background)
//		{
//			Background->setPosition(Pos);
//			//Window.draw(*Background);
//		}
//
//		if (Fill && Value > 0.f)
//		{
//			vec2f FillPos = Pos;
//			vec2f FillSize = Size;
//
//			switch (Direction)
//			{
//			case ProgressDirection::LeftToRight:
//				FillSize = { Size.x * Value, Size.y };
//				break;
//
//			case ProgressDirection::RightToLeft:
//				FillSize = { Size.x * Value, Size.y };
//				FillPos = Pos + vec2f{ Size.x - FillSize.x, 0.f };
//				break;
//
//			case ProgressDirection::TopToBottom:
//				FillSize = { Size.x, Size.y * Value };
//				break;
//
//			case ProgressDirection::BottomToTop:
//				FillSize = { Size.x, Size.y * Value };
//				FillPos = Pos + vec2f{ 0.f, Size.y - FillSize.y };
//				break;
//			}
//
//			Fill->setPosition(FillPos);
//			Fill->setSize(FillSize);
//			//Window.draw(*Fill);
//		}
//
//		Widget::Render(Window);
//	}
//}
