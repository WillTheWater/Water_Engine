// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/GUISubsystem.h"
#include "Framework/GameWindow.h"
#include "UI/Cursor/CursorSubsystem.h"
#include "UI/Widget/Widget.h"
#include "Input/InputBinding.h"
#include "Utility/Log.h"

namespace we
{
	GUISubsystem::GUISubsystem(GameWindow& Window, CursorSubsystem& InCursor)
		: Window{ Window }
		, Cursor{ InCursor }
	{
	}

	GUISubsystem::~GUISubsystem()
	{
		Clear();
	}

	void GUISubsystem::Clear()
	{
		Widgets.clear();
	}

	void GUISubsystem::ProcessEvent(const sf::Event& Event)
	{
		Event.visit([this](const auto& Type) { this->HandleEvent(Type); });
	}

	void GUISubsystem::Update(float DeltaTime)
	{
		for (auto& Widget : Widgets)
			if (Widget->IsVisible() && !Widget->GetParent())
				Widget->Update(DeltaTime);
	}

	void GUISubsystem::Render()
	{
		list<shared<Widget>> Roots;
		for (auto& Widget : Widgets)
			if (Widget->IsVisible() && !Widget->GetParent())
				Roots.push_back(Widget);

		std::sort(Roots.begin(), Roots.end(),
			[](const shared<Widget>& A, const shared<Widget>& B) {
				return A->GetZOrder() < B->GetZOrder();
			});

		for (auto& Widget : Roots)
			Widget->Render(Window);
	}

	shared<Widget> GUISubsystem::FindWidgetAt(const vec2f& WorldPoint) const
	{
		list<shared<Widget>> Roots;
		for (auto& W : Widgets)
		{
			if (W->IsVisible() && !W->GetParent())
				Roots.push_back(W);
		}

		std::sort(Roots.begin(), Roots.end(), [](const shared<Widget>& A, const shared<Widget>& B) {
			return A->GetZOrder() > B->GetZOrder();
			});

		for (auto& Root : Roots)
		{
			if (auto Hit = Root->FindDeepestChildAt(WorldPoint))
				return Hit;
		}

		return nullptr;
	}

	void GUISubsystem::HandleEvent(const sf::Event::MouseMoved& Mouse)
	{
		vec2f MousePos = Window.mapPixelToCoords(Mouse.position);

		// If pressed widget wants capture, send moves to it
		if (auto Pressed = PressedWidget.lock())
		{
			if (Pressed->ShouldCaptureMouse())
			{
				Pressed->OnMouseMoved(MousePos);
				return;
			}
		}

		shared<Widget> NewHovered = FindWidgetAt(MousePos);
		shared<Widget> OldHovered = HoveredWidget.lock();

		if (NewHovered != OldHovered)
		{
			if (OldHovered) OldHovered->OnMouseLeave();
			if (NewHovered) NewHovered->OnMouseEnter();
			HoveredWidget = NewHovered;
		}

		if (NewHovered) NewHovered->OnMouseMoved(MousePos);
	}

	void GUISubsystem::HandleEvent(const sf::Event::MouseButtonPressed&)
	{
		shared<Widget> Target = FindWidgetAt(Cursor.GetPosition());
		if (Target && Target->OnMouseButtonPressed())
			PressedWidget = Target;
	}

	void GUISubsystem::HandleEvent(const sf::Event::MouseButtonReleased&)
	{
		if (auto Target = PressedWidget.lock())
		{
			Target->OnMouseButtonReleased();
			PressedWidget.reset();
		}
	}

	void GUISubsystem::HandleEvent(const sf::Event::JoystickButtonPressed& Gamepad)
	{
		if (Input::HardwareToLogic(Gamepad.button, Gamepad.joystickId) != GamepadButton::South) return;

		shared<Widget> Target = FindWidgetAt(Cursor.GetPosition());
		if (Target && Target->OnMouseButtonPressed())
			PressedWidget = Target;
	}

	void GUISubsystem::HandleEvent(const sf::Event::JoystickButtonReleased& Gamepad)
	{
		if (Input::HardwareToLogic(Gamepad.button, Gamepad.joystickId) != GamepadButton::South) return;

		if (auto Target = PressedWidget.lock())
		{
			Target->OnMouseButtonReleased();
			PressedWidget.reset();
		}
	}
}