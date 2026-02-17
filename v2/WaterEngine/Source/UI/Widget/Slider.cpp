//#include "UI/Widget/Slider.h"
//#include "Subsystem/WindowSubsystem.h"
//#include "Framework/EngineSubsystem.h"
//
//namespace we
//{
//	Slider::Slider(EngineSubsystem& Subsystem, const vec2f& InTrackSize, const vec2f& InThumbSize)
//		: Widget(Subsystem), TrackSize(InTrackSize), ThumbSize(InThumbSize)
//	{
//		// Track rectangle
//		Track.emplace();
//		Track->setSize(TrackSize);
//		Track->setFillColor({ 200, 200, 200 });
//
//		// Thumb rectangle
//		Thumb.emplace();
//		Thumb->setSize(ThumbSize);
//		Thumb->setFillColor({ 100, 100, 100 });
//		Thumb->setOrigin(ThumbSize * 0.5f);
//
//		SetSize(TrackSize);
//		UpdateThumbPosition();
//	}
//
//	void Slider::SetValue(float NewValue)
//	{
//		Value = std::clamp(NewValue, 0.f, 1.f);
//		UpdateThumbPosition();
//		OnValueChanged.Broadcast(Value);
//	}
//
//	void Slider::Update(float DeltaTime)
//	{
//	}
//
//	void Slider::Render(GameWindow& Window)
//	{
//		if (!IsVisible()) return;
//
//		vec2f Pos = GetWorldPosition();
//
//		if (Track)
//		{
//			Track->setPosition(Pos);
//			//Window.draw(*Track);
//		}
//
//		if (Thumb)
//		{
//			float ThumbCenterX = ThumbSize.x * 0.5f + Value * (TrackSize.x - ThumbSize.x);
//			float ThumbCenterY = TrackSize.y * 0.5f;
//
//			Thumb->setPosition(Pos + vec2f{ ThumbCenterX, ThumbCenterY });
//			//Window.draw(*Thumb);
//		}
//
//		Widget::Render(Window);
//	}
//
//	void Slider::OnMouseEnter()
//	{
//		bHovered = true;
//		UpdateVisualState();
//	}
//
//	void Slider::OnMouseLeave()
//	{
//		bHovered = false;
//		UpdateVisualState();
//	}
//
//	bool Slider::OnMouseButtonPressed()
//	{
//		/*vec2f MousePos = Subsystem.Cursor->GetPosition();
//		vec2f TrackPos = GetWorldPosition();
//
//		if (MousePos.x < TrackPos.x || MousePos.x > TrackPos.x + TrackSize.x ||
//			MousePos.y < TrackPos.y || MousePos.y > TrackPos.y + TrackSize.y)
//			return false;
//
//		bGrabbed = true;
//
//		float LocalX = MousePos.x - TrackPos.x;
//		float MaxX = TrackSize.x - ThumbSize.x;
//		SetValue(LocalX / MaxX);
//
//		UpdateVisualState();*/
//		return true;
//	}
//
//	void Slider::OnMouseButtonReleased()
//	{
//		bGrabbed = false;
//		UpdateVisualState();
//	}
//
//	void Slider::OnMouseMoved(const vec2f& MousePos)
//	{
//		if (!bGrabbed) return;
//
//		vec2f TrackPos = GetWorldPosition();
//		float LocalX = MousePos.x - TrackPos.x;
//		float MaxX = TrackSize.x - ThumbSize.x;
//		SetValue(LocalX / MaxX);
//	}
//
//	void Slider::UpdateThumbPosition()
//	{
//		if (!Thumb) return;
//		vec2f Pos = GetWorldPosition();
//
//		float ThumbCenterX = ThumbSize.x * 0.5f + Value * (TrackSize.x - ThumbSize.x);
//		float ThumbCenterY = TrackSize.y * 0.5f;
//
//		Thumb->setPosition(Pos + vec2f{ ThumbCenterX, ThumbCenterY });
//	}
//
//	void Slider::UpdateVisualState()
//	{
//		if (!Thumb) return;
//
//		color ThumbColor;
//		if (bGrabbed)
//			ThumbColor = { 80, 80, 80 };
//		else if (bHovered)
//			ThumbColor = { 150, 150, 150 };
//		else
//			ThumbColor = { 100, 100, 100 };
//
//		Thumb->setFillColor(ThumbColor);
//	}
//}
