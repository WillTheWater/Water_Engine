// =============================================================================
// Water Engine v2.0.0
// Copyright (C) 2026 Will The Water
// =============================================================================

#include "Subsystem/GUISubsystem.h"
#include "Subsystem/RenderSubsystem.h"
#include "Subsystem/CursorSubsystem.h"
#include "Subsystem/AudioSubsystem.h"
#include "Subsystem/CameraSubsystem.h"
#include "Framework/EngineSubsystem.h"
#include "UI/Widget/Widget.h"
#include "UI/Widget/Button.h"
#include "UI/Widget/Panel.h"
#include "UI/Widget/VerticalBox.h"
#include "UI/Widget/HorizontalBox.h"
#include "UI/Widget/GridBox.h"
#include "UI/Widget/AutoPanel.h"
#include "UI/Widget/Spacer.h"
#include "UI/Widget/CheckBox.h"
#include "UI/Widget/Slider.h"
#include "UI/Widget/ProgressBar.h"
#include "UI/Widget/TextBlock.h"
#include "Framework/World/RenderTypes.h"
#include "UI/UIUtility.h"
#include "Utility/Log.h"
#include "EngineConfig.h"

namespace we
{
	GUISubsystem::GUISubsystem(EngineSubsystem& InSubsystem)
		: Subsystem{ InSubsystem }
	{
	}

	GUISubsystem::~GUISubsystem()
	{
		Clear();
	}

	shared<Button> GUISubsystem::CreateButton(
		const string& Label,
		const vec2f& Size,
		color FillColor,
		color OutlineColor,
		float OutlineThickness,
		EWidgetSpace Space)
	{
		auto NewButton = make_shared<Button>(Label, Size, FillColor, OutlineColor, OutlineThickness);
		NewButton->SetWidgetSpace(Space);
		Widgets.push_back(NewButton);
		return NewButton;
	}

	shared<Button> GUISubsystem::CreateTextureButton(
		const string& Label,
		const string& TexturePath,
		const vec2f& Size,
		bool bInUseColorTint,
		EWidgetSpace Space)
	{
		auto NewButton = make_shared<Button>(Label, TexturePath, Size, bInUseColorTint);
		NewButton->SetWidgetSpace(Space);
		Widgets.push_back(NewButton);
		return NewButton;
	}

	shared<Panel> GUISubsystem::CreatePanel(
		const vec2f& Size,
		color FillColor,
		color OutlineColor,
		float OutlineThickness,
		EWidgetSpace Space)
	{
		auto NewPanel = make_shared<Panel>(Size, FillColor, OutlineColor, OutlineThickness);
		NewPanel->SetWidgetSpace(Space);
		Widgets.push_back(NewPanel);
		return NewPanel;
	}

	shared<Panel> GUISubsystem::CreateTexturePanel(
		const string& TexturePath,
		const vec2f& Size,
		EWidgetSpace Space)
	{
		auto NewPanel = make_shared<Panel>(TexturePath, Size);
		NewPanel->SetWidgetSpace(Space);
		Widgets.push_back(NewPanel);
		return NewPanel;
	}

	shared<VerticalBox> GUISubsystem::CreateVerticalBox(float Padding, EWidgetSpace Space)
	{
		auto NewBox = make_shared<VerticalBox>(Padding);
		NewBox->SetWidgetSpace(Space);
		Widgets.push_back(NewBox);
		return NewBox;
	}

	shared<HorizontalBox> GUISubsystem::CreateHorizontalBox(float Padding, EWidgetSpace Space)
	{
		auto NewBox = make_shared<HorizontalBox>(Padding);
		NewBox->SetWidgetSpace(Space);
		Widgets.push_back(NewBox);
		return NewBox;
	}

	shared<GridBox> GUISubsystem::CreateGridBox(uint Columns, float Padding, EWidgetSpace Space)
	{
		auto NewBox = make_shared<GridBox>(Columns, Padding);
		NewBox->SetWidgetSpace(Space);
		Widgets.push_back(NewBox);
		return NewBox;
	}

	shared<AutoPanel> GUISubsystem::CreateAutoPanel(
		color FillColor,
		color OutlineColor,
		float OutlineThickness,
		float Spacing,
		EWidgetSpace Space)
	{
		auto NewPanel = make_shared<AutoPanel>(FillColor, OutlineColor, OutlineThickness, Spacing);
		NewPanel->SetWidgetSpace(Space);
		Widgets.push_back(NewPanel);
		return NewPanel;
	}

	shared<Spacer> GUISubsystem::CreateSpacer(float Width, float Height, EWidgetSpace Space)
	{
		auto NewSpacer = make_shared<Spacer>(Width, Height);
		NewSpacer->SetWidgetSpace(Space);
		Widgets.push_back(NewSpacer);
		return NewSpacer;
	}

	shared<CheckBox> GUISubsystem::CreateCheckBox(
		bool bInitialState,
		float BoxSize,
		CheckBoxShape Shape,
		EWidgetSpace Space)
	{
		auto NewCheckBox = make_shared<CheckBox>(bInitialState, BoxSize, Shape);
		NewCheckBox->SetWidgetSpace(Space);
		Widgets.push_back(NewCheckBox);
		return NewCheckBox;
	}

	shared<CheckBox> GUISubsystem::CreateTextureCheckBox(
		const string& TexturePath,
		bool bInitialState,
		float BoxSize,
		EWidgetSpace Space)
	{
		auto NewCheckBox = make_shared<CheckBox>(TexturePath, bInitialState, BoxSize);
		NewCheckBox->SetWidgetSpace(Space);
		Widgets.push_back(NewCheckBox);
		return NewCheckBox;
	}

	shared<Slider> GUISubsystem::CreateSlider(
		float TrackWidth,
		float TrackHeight,
		float ThumbWidth,
		float ThumbHeight,
		SliderOrientation Orient,
		EWidgetSpace Space)
	{
		auto NewSlider = make_shared<Slider>(TrackWidth, TrackHeight, ThumbWidth, ThumbHeight, Orient);
		NewSlider->SetWidgetSpace(Space);
		Widgets.push_back(NewSlider);
		return NewSlider;
	}

	shared<Slider> GUISubsystem::CreateTextureTrackSlider(
		const string& TrackTexturePath,
		float ThumbWidth,
		float ThumbHeight,
		SliderOrientation Orient,
		EWidgetSpace Space)
	{
		auto NewSlider = make_shared<Slider>(TrackTexturePath, ThumbWidth, ThumbHeight, Orient);
		NewSlider->SetWidgetSpace(Space);
		Widgets.push_back(NewSlider);
		return NewSlider;
	}

	shared<Slider> GUISubsystem::CreateTextureThumbSlider(
		float TrackWidth,
		float TrackHeight,
		const string& ThumbTexturePath,
		SliderOrientation Orient,
		EWidgetSpace Space)
	{
		auto NewSlider = make_shared<Slider>(TrackWidth, TrackHeight, ThumbTexturePath, Orient);
		NewSlider->SetWidgetSpace(Space);
		Widgets.push_back(NewSlider);
		return NewSlider;
	}

	shared<Slider> GUISubsystem::CreateTextureSlider(
		const string& TrackTexturePath,
		const string& ThumbTexturePath,
		SliderOrientation Orient,
		EWidgetSpace Space)
	{
		auto NewSlider = make_shared<Slider>(TrackTexturePath, ThumbTexturePath, Orient);
		NewSlider->SetWidgetSpace(Space);
		Widgets.push_back(NewSlider);
		return NewSlider;
	}

	shared<ProgressBar> GUISubsystem::CreateProgressBar(
		const vec2f& Size,
		float InitialValue,
		EWidgetSpace Space)
	{
		auto NewBar = make_shared<ProgressBar>(Size, InitialValue);
		NewBar->SetWidgetSpace(Space);
		Widgets.push_back(NewBar);
		return NewBar;
	}

	shared<ProgressBar> GUISubsystem::CreateTextureBackgroundProgressBar(
		const string& BackgroundTexturePath,
		const vec2f& Size,
		float InitialValue,
		EWidgetSpace Space)
	{
		auto NewBar = make_shared<ProgressBar>(BackgroundTexturePath, Size, InitialValue);
		NewBar->SetWidgetSpace(Space);
		Widgets.push_back(NewBar);
		return NewBar;
	}

	shared<ProgressBar> GUISubsystem::CreateTextureFillProgressBar(
		const vec2f& Size,
		const string& FillTexturePath,
		float InitialValue,
		EWidgetSpace Space)
	{
		auto NewBar = make_shared<ProgressBar>(Size, FillTexturePath, InitialValue);
		NewBar->SetWidgetSpace(Space);
		Widgets.push_back(NewBar);
		return NewBar;
	}

	shared<ProgressBar> GUISubsystem::CreateTextureProgressBar(
		const string& BackgroundTexturePath,
		const string& FillTexturePath,
		float InitialValue,
		EWidgetSpace Space)
	{
		auto NewBar = make_shared<ProgressBar>(BackgroundTexturePath, FillTexturePath, InitialValue);
		NewBar->SetWidgetSpace(Space);
		Widgets.push_back(NewBar);
		return NewBar;
	}

	shared<TextBlock> GUISubsystem::CreateTextBlock(
		const string& Text,
		float WrapWidth,
		uint CharSize,
		EWidgetSpace Space)
	{
		auto NewText = make_shared<TextBlock>(Text, WrapWidth, CharSize);
		NewText->SetWidgetSpace(Space);
		Widgets.push_back(NewText);
		return NewText;
	}

	void GUISubsystem::Clear()
	{
		Widgets.clear();
		HoveredWidget.reset();
		PressedWidget.reset();
		FocusedWidget.reset();
		bHasMouseFocus = false;
		bRenderOrderDirty = true;
		CachedRenderDepths.clear();
		LastWidgetCount = 0;
	}

	bool GUISubsystem::HandleEvent(const sf::Event& Event)
	{
		bool bConsumed = false;

		if (auto MouseMoved = Event.getIf<sf::Event::MouseMoved>())
		{
			HandleMouseMoved(*MouseMoved);
			bConsumed = bHasMouseFocus;
		}
		else if (auto MousePressed = Event.getIf<sf::Event::MouseButtonPressed>())
		{
			if (MousePressed->button == sf::Mouse::Button::Left)
			{
				HandleMousePressed(*MousePressed);
				bConsumed = bHasMouseFocus;
			}
		}
		else if (auto MouseReleased = Event.getIf<sf::Event::MouseButtonReleased>())
		{
			if (MouseReleased->button == sf::Mouse::Button::Left)
			{
				HandleMouseReleased(*MouseReleased);
			}
		}
		else if (auto KeyPressed = Event.getIf<sf::Event::KeyPressed>())
		{
			HandleKeyPressed(*KeyPressed);
			bConsumed = (FocusedWidget.lock() != nullptr) &&
				(KeyPressed->scancode == sf::Keyboard::Scan::Tab ||
				 KeyPressed->scancode == sf::Keyboard::Scan::Enter ||
				 KeyPressed->scancode == sf::Keyboard::Scan::Escape);
		}
		// Joystick/Gamepad input is handled via Input subsystem callbacks, not raw events
		else if (auto TextEntered = Event.getIf<sf::Event::TextEntered>())
		{
			HandleTextEntered(*TextEntered);
			bConsumed = (FocusedWidget.lock() != nullptr);
		}

		return bConsumed;
	}

	vec2f GUISubsystem::GetMousePosition() const
	{
		// Cursor is always in window pixel coordinates
		return Subsystem.Cursor->GetPosition();
	}

	void GUISubsystem::HandleMouseMoved(const sf::Event::MouseMoved& Mouse)
	{
		vec2f MousePos = GetMousePosition();
		UpdateHoverState(MousePos);

		// Notify pressed widget of drag (for sliders and other draggable widgets)
		if (auto Pressed = PressedWidget.lock())
		{
			vec2f TransformedPos = TransformPointForWidget(MousePos, Pressed);
			Pressed->OnDrag(TransformedPos);
		}
	}

	void GUISubsystem::HandleMousePressed(const sf::Event::MouseButtonPressed& Btn)
	{
		vec2f MousePos = GetMousePosition();
		auto Target = FindWidgetAt(MousePos);

		if (Target && Target->IsVisible())
		{
			if (auto OldFocus = FocusedWidget.lock())
			{
				OldFocus->OnFocusLost.Broadcast();
			}

			FocusedWidget = Target;
			Target->OnFocusGained.Broadcast();

			PressedWidget = Target;
			Target->SetPressed(true);
			
			vec2f TransformedPos = TransformPointForWidget(MousePos, Target);
			Target->OnPressed(TransformedPos);

			const string& PressedSound = Target->GetPressedSound();
			if (Subsystem.Audio && !PressedSound.empty())
			{
				Subsystem.Audio->PlaySFX(PressedSound);
			}
		}
		else
		{
			if (auto OldFocus = FocusedWidget.lock())
			{
				OldFocus->OnFocusLost.Broadcast();
				FocusedWidget.reset();
			}
		}
	}

	void GUISubsystem::HandleMouseReleased(const sf::Event::MouseButtonReleased& Btn)
	{
		if (auto Pressed = PressedWidget.lock())
		{
			Pressed->SetPressed(false);

			vec2f MousePos = GetMousePosition();
			vec2f TransformedPos = TransformPointForWidget(MousePos, Pressed);
			Pressed->OnReleased(TransformedPos);

			if (Pressed->Contains(TransformedPos))
			{
				Pressed->OnClicked.Broadcast();
				
				const string& ClickSound = Pressed->GetClickSound();
				if (Subsystem.Audio && !ClickSound.empty())
				{
					Subsystem.Audio->PlaySFX(ClickSound);
				}
			}
		}
		PressedWidget.reset();
	}

	void GUISubsystem::HandleKeyPressed(const sf::Event::KeyPressed& Key)
	{
		// Only process GUI navigation keys if we have visible widgets
		bool bHasVisibleWidgets = false;
		for (const auto& Widget : Widgets)
		{
			if (Widget && Widget->IsVisible())
			{
				bHasVisibleWidgets = true;
				break;
			}
		}
		
		if (!bHasVisibleWidgets)
		{
			return;  // No visible UI, ignore GUI input
		}
		
		if (Key.scancode == sf::Keyboard::Scan::Tab)
		{
			if (Key.shift)
				NavigatePrevious();
			else
				NavigateNext();
		}
		else if (Key.scancode == sf::Keyboard::Scan::Enter || Key.scancode == sf::Keyboard::Scan::Space)
		{
			ActivateFocused();
		}
		else if (Key.scancode == sf::Keyboard::Scan::Escape)
		{
			// Cancel/Close handled by game
		}
	}

	// =========================================================================
	// Gamepad Input (via Input subsystem - NOT raw SFML events)
	// =========================================================================

	void GUISubsystem::OnGamepadConfirmPressed()
	{
		vec2f CursorPos = GetMousePosition();
		if (auto Hovered = FindWidgetAt(CursorPos))
		{
			if (Hovered->IsVisible())
			{
				if (auto OldFocus = FocusedWidget.lock())
				{
					OldFocus->OnFocusLost.Broadcast();
				}
				FocusedWidget = Hovered;
				Hovered->OnFocusGained.Broadcast();

				PressedWidget = Hovered;
				Hovered->SetPressed(true);
				
				vec2f TransformedPos = TransformPointForWidget(CursorPos, Hovered);
				Hovered->OnPressed(TransformedPos);
				
				const string& PressedSound = Hovered->GetPressedSound();
				if (Subsystem.Audio && !PressedSound.empty())
				{
					Subsystem.Audio->PlaySFX(PressedSound);
				}
			}
		}
		else
		{
			// No widget under cursor, fall back to focused widget
			if (auto Focused = FocusedWidget.lock())
			{
				if (Focused->IsVisible())
				{
					PressedWidget = Focused;
					Focused->SetPressed(true);
					
					vec2f Center = Focused->GetWorldPosition() + Focused->GetSize() / 2.0f;
					Focused->OnPressed(Center);
					
					const string& PressedSound = Focused->GetPressedSound();
					if (Subsystem.Audio && !PressedSound.empty())
					{
						Subsystem.Audio->PlaySFX(PressedSound);
					}
				}
			}
		}
	}

	void GUISubsystem::OnGamepadConfirmReleased()
	{
		if (auto Pressed = PressedWidget.lock())
		{
			Pressed->SetPressed(false);
			
			vec2f CursorPos = GetMousePosition();
			vec2f TransformedPos = TransformPointForWidget(CursorPos, Pressed);
			Pressed->OnReleased(TransformedPos);
			
			if (Pressed->Contains(TransformedPos))
			{
				Pressed->OnClicked.Broadcast();
				
				const string& ClickSound = Pressed->GetClickSound();
				if (Subsystem.Audio && !ClickSound.empty())
				{
					Subsystem.Audio->PlaySFX(ClickSound);
				}
			}
		}
		PressedWidget.reset();
	}

	void GUISubsystem::OnGamepadCancel()
	{
		// Cancel/Back action - can be bound by game
	}

	void GUISubsystem::OnGamepadNavigateNext()
	{
		NavigateNext();
	}

	void GUISubsystem::OnGamepadNavigatePrevious()
	{
		NavigatePrevious();
	}

	void GUISubsystem::HandleTextEntered(const sf::Event::TextEntered& Text)
	{
		if (auto Focused = FocusedWidget.lock())
		{
			// Text input passed to focused widget
		}
	}

	void GUISubsystem::NavigateNext()
	{
		auto Focusable = GetFocusableWidgets();
		if (Focusable.empty()) return;

		auto Current = FocusedWidget.lock();
		size_t CurrentIndex = 0;
		
		for (size_t i = 0; i < Focusable.size(); ++i)
		{
			if (Focusable[i] == Current)
			{
				CurrentIndex = i;
				break;
			}
		}

		size_t NextIndex = (CurrentIndex + 1) % Focusable.size();
		auto Next = Focusable[NextIndex];

		if (Next && Next != Current)
		{
			if (Current)
			{
				Current->OnFocusLost.Broadcast();
			}
			FocusedWidget = Next;
			Next->OnFocusGained.Broadcast();
			SnapCursorToWidget(Next);
		}
	}

	void GUISubsystem::NavigatePrevious()
	{
		auto Focusable = GetFocusableWidgets();
		if (Focusable.empty()) return;

		auto Current = FocusedWidget.lock();
		size_t CurrentIndex = 0;
		
		for (size_t i = 0; i < Focusable.size(); ++i)
		{
			if (Focusable[i] == Current)
			{
				CurrentIndex = i;
				break;
			}
		}

		size_t PrevIndex = (CurrentIndex == 0) ? Focusable.size() - 1 : CurrentIndex - 1;
		auto Prev = Focusable[PrevIndex];

		if (Prev && Prev != Current)
		{
			if (Current)
			{
				Current->OnFocusLost.Broadcast();
			}
			FocusedWidget = Prev;
			Prev->OnFocusGained.Broadcast();
			SnapCursorToWidget(Prev);
		}
	}

	void GUISubsystem::ActivateFocused()
	{
		if (auto Focused = FocusedWidget.lock())
		{
			// Only activate if the widget is visible and focusable
			if (Focused->IsVisible() && Focused->IsFocusable())
			{
				Focused->OnClicked.Broadcast();
			}
		}
	}

	void GUISubsystem::Update(float DeltaTime)
	{
		for (auto& Widget : Widgets)
		{
			if (Widget->IsVisible())
			{
				Widget->Update(DeltaTime);
			}
		}

		// Update hover state every frame (for gamepad cursor movement)
		vec2f CursorPos = GetMousePosition();
		UpdateHoverState(CursorPos);

		// Handle dragging for pressed widget (gamepad cursor movement)
		if (auto Pressed = PressedWidget.lock())
		{
			vec2f TransformedPos = TransformPointForWidget(CursorPos, Pressed);
			Pressed->OnDrag(TransformedPos);
		}
	}

	void GUISubsystem::Render()
	{
		// Render screen-space widgets first (to ScreenUI layer)
		vector<RenderDepth> ScreenRenderDepths;
		for (auto& Widget : Widgets)
		{
			if (Widget && Widget->IsVisible() && Widget->GetWidgetSpace() == EWidgetSpace::Screen)
			{
				Widget->CollectRenderDepths(ScreenRenderDepths);
			}
		}
		std::sort(ScreenRenderDepths.begin(), ScreenRenderDepths.end());
		for (const auto& Entry : ScreenRenderDepths)
		{
			Subsystem.Render->Draw(*Entry.Drawable, ERenderLayer::ScreenUI);
		}

		// Render world-space widgets second (to WorldUI layer)
		vector<RenderDepth> WorldRenderDepths;
		for (auto& Widget : Widgets)
		{
			if (Widget && Widget->IsVisible() && Widget->GetWidgetSpace() == EWidgetSpace::World)
			{
				Widget->CollectRenderDepths(WorldRenderDepths);
			}
		}
		std::sort(WorldRenderDepths.begin(), WorldRenderDepths.end());
		for (const auto& Entry : WorldRenderDepths)
		{
			Subsystem.Render->Draw(*Entry.Drawable, ERenderLayer::WorldUI);
		}
	}

	void GUISubsystem::UpdateHoverState(const vec2f& MousePos)
	{
		auto NewHovered = FindWidgetAt(MousePos);
		auto OldHovered = HoveredWidget.lock();

		if (NewHovered != OldHovered)
		{
			if (OldHovered)
			{
				OldHovered->OnHoverLost.Broadcast();
			}
			if (NewHovered)
			{
				NewHovered->OnHoverGained.Broadcast();
				
				const string& HoverSound = NewHovered->GetHoverSound();
				if (Subsystem.Audio && !HoverSound.empty())
				{
					Subsystem.Audio->PlaySFX(HoverSound);
				}
			}

			HoveredWidget = NewHovered;
			bHasMouseFocus = (NewHovered != nullptr);
		}
	}

	vec2f GUISubsystem::TransformPointForWidget(const vec2f& ScreenPoint, shared<Widget> Widget) const
	{
		if (!Widget)
			return ScreenPoint;

		if (Widget->GetWidgetSpace() == EWidgetSpace::Screen)
		{
			// Screen widgets: Map window pixels to render resolution coordinates
			// This accounts for letterboxing when window size differs from render resolution
			vec2i PixelPos(static_cast<int>(ScreenPoint.x), static_cast<int>(ScreenPoint.y));
			return Subsystem.Render->MapPixelToCoords(PixelPos);
		}
		else
		{
			// World widgets need cursor converted to world coordinates
			return Subsystem.Cursor->GetWorldPosition(*Subsystem.Camera);
		}
	}

	shared<Widget> GUISubsystem::FindWidgetAt(const vec2f& ScreenPoint) const
	{
		// Find the visible widget with maximum render depth that contains the point
		shared<Widget> TopWidget = nullptr;
		float MaxDepth = -FLT_MAX;

		for (auto& Widget : Widgets)
		{
			if (!Widget || !Widget->IsVisible())
				continue;

			// Transform point to widget's coordinate space
			vec2f TestPoint = TransformPointForWidget(ScreenPoint, Widget);

			if (Widget->Contains(TestPoint) && Widget->GetRenderDepth() > MaxDepth)
			{
				MaxDepth = Widget->GetRenderDepth();
				TopWidget = Widget;
			}
		}

		// If we found a containing widget, let it find the deepest child
		if (TopWidget)
		{
			vec2f TestPoint = TransformPointForWidget(ScreenPoint, TopWidget);
			return TopWidget->FindDeepestChildAt(TestPoint);
		}

		return nullptr;
	}

	vector<shared<Widget>> GUISubsystem::GetFocusableWidgets() const
	{
		vector<shared<Widget>> Focusable;
		for (auto& Widget : Widgets)
		{
			if (Widget && Widget->IsVisible() && Widget->IsFocusable())
			{
				Focusable.push_back(Widget);
			}
		}
		return Focusable;
	}

	void GUISubsystem::SnapCursorToWidget(shared<Widget> TargetWidget)
	{
		if (!TargetWidget) return;

		vec2f CursorPos = TargetWidget->GetFocusPoint();

		if (TargetWidget->GetWidgetSpace() == EWidgetSpace::Screen)
		{
			// Screen widget: use position directly
			Subsystem.Cursor->SetPosition(CursorPos);
		}
		else
		{
			// World widget: need to convert world position to screen
			// Get camera view
			CameraView View;
			if (Subsystem.Camera->GetCurrentView(View))
			{
				// Calculate aspect ratio
				float AspectRatio = EC.AspectRatio.x / EC.AspectRatio.y;
				vec2f ViewSize = View.GetViewSize(AspectRatio);

				// Convert world position to normalized device coordinates
				vec2f WorldPos = CursorPos;
				float NormalizedX = (WorldPos.x - View.Position.x) / (ViewSize.x * 0.5f);
				float NormalizedY = (WorldPos.y - View.Position.y) / (ViewSize.y * 0.5f);

				// Convert to window pixels
				vec2f WindowSize = vec2f(Subsystem.Window->getSize());
				vec2f ScreenPos;
				ScreenPos.x = (NormalizedX + 1.0f) * 0.5f * WindowSize.x;
				ScreenPos.y = (NormalizedY + 1.0f) * 0.5f * WindowSize.y;

				Subsystem.Cursor->SetPosition(ScreenPos);
			}
		}
		
		// Update hovered widget based on new cursor position
		UpdateHoverState(CursorPos);
	}
}
