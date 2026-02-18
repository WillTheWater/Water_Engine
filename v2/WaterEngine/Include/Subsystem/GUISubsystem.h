// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "UI/Widget/CheckBox.h"
#include "UI/Widget/Slider.h"
#include "UI/Widget/ProgressBar.h"
#include "UI/Widget/TextBlock.h"

namespace we
{
	class Widget;
	class Button;
	class Panel;
	class AutoPanel;
	class VerticalBox;
	class HorizontalBox;
	class GridBox;
	class Spacer;
	class CheckBox;
	class Slider;
	class ProgressBar;
	class TextBlock;
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

		// Factory: Rectangle panel
		shared<Panel> CreatePanel(
			const vec2f& Size = { 300.f, 200.f },
			color FillColor = color{ 50, 50, 50, 200 },
			color OutlineColor = color{ 100, 100, 100 },
			float OutlineThickness = 1.f);

		// Factory: Texture panel
		shared<Panel> CreateTexturePanel(
			const string& TexturePath,
			const vec2f& Size = { 0.f, 0.f });

		// Factory: Vertical layout box
		shared<VerticalBox> CreateVerticalBox(float Padding = 10.f);

		// Factory: Horizontal layout box
		shared<HorizontalBox> CreateHorizontalBox(float Padding = 10.f);

		// Factory: Grid layout box
		shared<GridBox> CreateGridBox(uint Columns = 2, float Padding = 10.f);

		// Factory: Auto-sizing panel with child alignment
		shared<AutoPanel> CreateAutoPanel(
			color FillColor = color{ 50, 50, 50, 200 },
			color OutlineColor = color{ 100, 100, 100 },
			float OutlineThickness = 1.f,
			float Spacing = 10.f);

		// Factory: Spacer widget
		shared<Spacer> CreateSpacer(float Width = 10.f, float Height = 10.f);

		// Factory: Shape-based checkbox
		shared<CheckBox> CreateCheckBox(
			bool bInitialState = false,
			float BoxSize = 24.f,
			CheckBoxShape Shape = CheckBoxShape::Rectangle);

		// Factory: Texture-based checkbox
		shared<CheckBox> CreateTextureCheckBox(
			const string& TexturePath,
			bool bInitialState = false,
			float BoxSize = 0.f);

		// Factory: Rectangle slider (horizontal or vertical)
		shared<Slider> CreateSlider(
			float TrackWidth = 200.f,
			float TrackHeight = 10.f,
			float ThumbWidth = 20.f,
			float ThumbHeight = 30.f,
			SliderOrientation Orient = SliderOrientation::Horizontal);

		// Factory: Texture track + rectangle thumb slider
		shared<Slider> CreateTextureTrackSlider(
			const string& TrackTexturePath,
			float ThumbWidth = 20.f,
			float ThumbHeight = 30.f,
			SliderOrientation Orient = SliderOrientation::Horizontal);

		// Factory: Rectangle track + texture thumb slider
		shared<Slider> CreateTextureThumbSlider(
			float TrackWidth = 200.f,
			float TrackHeight = 10.f,
			const string& ThumbTexturePath = "",
			SliderOrientation Orient = SliderOrientation::Horizontal);

		// Factory: Texture track + texture thumb slider
		shared<Slider> CreateTextureSlider(
			const string& TrackTexturePath,
			const string& ThumbTexturePath,
			SliderOrientation Orient = SliderOrientation::Horizontal);

		// Factory: Rectangle progress bar
		shared<ProgressBar> CreateProgressBar(
			const vec2f& Size = { 200.f, 24.f },
			float InitialValue = 0.f);

		// Factory: Texture background + rectangle fill progress bar
		shared<ProgressBar> CreateTextureBackgroundProgressBar(
			const string& BackgroundTexturePath,
			const vec2f& Size = { 200.f, 24.f },
			float InitialValue = 0.f);

		// Factory: Rectangle background + texture fill progress bar
		shared<ProgressBar> CreateTextureFillProgressBar(
			const vec2f& Size,
			const string& FillTexturePath,
			float InitialValue = 0.f);

		// Factory: Texture background + texture fill progress bar
		shared<ProgressBar> CreateTextureProgressBar(
			const string& BackgroundTexturePath,
			const string& FillTexturePath,
			float InitialValue = 0.f);

		// Factory: TextBlock with auto-wrap
		shared<TextBlock> CreateTextBlock(
			const string& Text = "",
			float WrapWidth = 0.f,
			uint CharSize = 24);

		// Handle raw input events, returns true if consumed by GUI
		bool HandleEvent(const sf::Event& Event);

		void Update(float DeltaTime);
		void Render();  // Uses Subsystem.Render internally

		void Clear();

		// Dirty flag for render order optimization
		void MarkRenderDirty() { bRenderOrderDirty = true; }

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
		bool bRenderOrderDirty = true;  // Force initial sort
		vector<RenderDepth> CachedRenderDepths;  // Cached sorted drawables
		size_t LastWidgetCount = 0;  // Track widget count changes
	};
}