// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
	class Widget;
	class Button;
	struct EngineSubsystem;

	class GUISubsystem
	{
	public:
		// GUI-specific input actions (internal to GUI system)
		enum class GUIAction
		{
			Click,
			Next,      // Next focusable widget (Tab, RB, Right, Down)
			Previous,  // Previous focusable widget (Shift+Tab, LB, Left, Up)
			Confirm,   // Activate focused widget (Enter, Space, South)
			Cancel     // Back/Close (Escape, East, Back)
		};

	public:
		explicit GUISubsystem(EngineSubsystem& InSubsystem);
		~GUISubsystem();

		// Factory: Rectangle button with text
		shared<Button> CreateButton(
			const string& Label = "",
			const vec2f& Size = { 150.f, 50.f },
			color FillColor = color{ 200, 200, 200 },
			color OutlineColor = color::Black,
			float OutlineThickness = 2.f);

		// Factory: Texture button with text
		// bUseColorTint: true = tint texture with colors, false = use texture rects (default atlas layout)
		shared<Button> CreateTextureButton(
			const string& Label = "",
			const string& TexturePath = "",
			const vec2f& Size = { 0.f, 0.f },
			bool bUseColorTint = false);

		// Handle raw input events, returns true if consumed by GUI
		bool HandleEvent(const sf::Event& Event);

		void Update(float DeltaTime);
		void Render();  // Uses Subsystem.Render internally

		void Clear();

	private:
		// Event handlers
		void HandleMouseMoved(const sf::Event::MouseMoved& Mouse);
		void HandleMousePressed(const sf::Event::MouseButtonPressed& Btn);
		void HandleMouseReleased(const sf::Event::MouseButtonReleased& Btn);
		void HandleKeyPressed(const sf::Event::KeyPressed& Key);
		void HandleJoystickPressed(const sf::Event::JoystickButtonPressed& Btn);
		void HandleTextEntered(const sf::Event::TextEntered& Text);

		// Navigation
		void NavigateNext();
		void NavigatePrevious();
		void ActivateFocused();

		shared<Widget> FindWidgetAt(const vec2f& ScreenPoint) const;
		shared<Widget> FindNextFocusable() const;
		shared<Widget> FindPreviousFocusable() const;
		void UpdateHoverState(const vec2f& MousePos);
		vec2f GetMousePosition() const;

	private:
		EngineSubsystem& Subsystem;
		vector<shared<Widget>> Widgets;

		weak<Widget> HoveredWidget;
		weak<Widget> PressedWidget;
		weak<Widget> FocusedWidget;

		bool bHasMouseFocus = false;  // True if mouse is over any widget
	};
}