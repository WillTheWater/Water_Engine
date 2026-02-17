// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/GUISubsystem.h"
#include "Subsystem/RenderSubsystem.h"
#include "Subsystem/CursorSubsystem.h"
#include "Subsystem/AudioSubsystem.h"
#include "Framework/EngineSubsystem.h"
#include "UI/Widget/Widget.h"
#include "UI/Widget/Button.h"
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
		float OutlineThickness)
	{
		auto NewButton = make_shared<Button>(Label, Size, FillColor, OutlineColor, OutlineThickness);
		Widgets.push_back(NewButton);
		return NewButton;
	}

	shared<Button> GUISubsystem::CreateTextureButton(
		const string& Label,
		const string& TexturePath,
		const vec2f& Size,
		bool bUseColorTint)
	{
		auto NewButton = make_shared<Button>(Label, TexturePath, Size, bUseColorTint);
		Widgets.push_back(NewButton);
		return NewButton;
	}

	void GUISubsystem::Clear()
	{
		Widgets.clear();
		HoveredWidget.reset();
		PressedWidget.reset();
		FocusedWidget.reset();
		bHasMouseFocus = false;
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
		else if (auto JoystickPressed = Event.getIf<sf::Event::JoystickButtonPressed>())
		{
			HandleJoystickPressed(*JoystickPressed);
			bConsumed = (FocusedWidget.lock() != nullptr);
		}
		else if (auto TextEntered = Event.getIf<sf::Event::TextEntered>())
		{
			HandleTextEntered(*TextEntered);
			bConsumed = (FocusedWidget.lock() != nullptr);
		}

		return bConsumed;
	}

	vec2f GUISubsystem::GetMousePosition() const
	{
		vec2f PixelPos = Subsystem.Cursor->GetPixelPosition();
		
		vec2u WindowSize = Subsystem.Window->getSize();
		float ScaleX = static_cast<float>(EC.RenderResolution.x) / WindowSize.x;
		float ScaleY = static_cast<float>(EC.RenderResolution.y) / WindowSize.y;
		
		return vec2f(PixelPos.x * ScaleX, PixelPos.y * ScaleY);
	}

	void GUISubsystem::HandleMouseMoved(const sf::Event::MouseMoved& Mouse)
	{
		vec2f MousePos = GetMousePosition();
		UpdateHoverState(MousePos);
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
			if (Pressed->Contains(MousePos))
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

	void GUISubsystem::HandleJoystickPressed(const sf::Event::JoystickButtonPressed& Btn)
	{
		if (Btn.button == 0)
		{
			ActivateFocused();
		}
		else if (Btn.button == 1)
		{
			// Cancel
		}
		else if (Btn.button == 4)
		{
			NavigatePrevious();
		}
		else if (Btn.button == 5)
		{
			NavigateNext();
		}
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
		if (auto Next = FindNextFocusable())
		{
			if (auto Current = FocusedWidget.lock())
			{
				Current->OnFocusLost.Broadcast();
			}

			FocusedWidget = Next;
			Next->OnFocusGained.Broadcast();
		}
	}

	void GUISubsystem::NavigatePrevious()
	{
		if (auto Prev = FindPreviousFocusable())
		{
			if (auto Current = FocusedWidget.lock())
			{
				Current->OnFocusLost.Broadcast();
			}

			FocusedWidget = Prev;
			Prev->OnFocusGained.Broadcast();
		}
	}

	void GUISubsystem::ActivateFocused()
	{
		if (auto Focused = FocusedWidget.lock())
		{
			Focused->OnClicked.Broadcast();
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
	}

	void GUISubsystem::Render()
	{
		static vector<RenderDepth> GUIDepths;
		GUIDepths.clear();

		for (auto& Widget : Widgets)
		{
			if (Widget && Widget->IsVisible())
			{
				Widget->CollectRenderDepths(GUIDepths);
			}
		}

		std::sort(GUIDepths.begin(), GUIDepths.end());

		for (const auto& Entry : GUIDepths)
		{
			Subsystem.Render->Draw(*Entry.Drawable, ERenderLayer::UI);
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
				OldHovered->OnFocusLost.Broadcast();
			}
			if (NewHovered)
			{
				NewHovered->OnFocusGained.Broadcast();
				
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

	shared<Widget> GUISubsystem::FindWidgetAt(const vec2f& ScreenPoint) const
	{
		vector<shared<Widget>> Visible;
		for (auto& Widget : Widgets)
		{
			if (Widget && Widget->IsVisible())
			{
				Visible.push_back(Widget);
			}
		}

		std::sort(Visible.begin(), Visible.end(),
			[](const shared<Widget>& A, const shared<Widget>& B)
			{
				return A->GetRenderDepth() > B->GetRenderDepth();
			});

		for (auto& Widget : Visible)
		{
			if (auto Hit = Widget->FindDeepestChildAt(ScreenPoint))
			{
				return Hit;
			}
		}
		return nullptr;
	}

	shared<Widget> GUISubsystem::FindNextFocusable() const
	{
		auto Current = FocusedWidget.lock();
		float CurrentDepth = Current ? Current->GetRenderDepth() : 0.f;

		shared<Widget> BestCandidate;
		float BestDepth = FLT_MAX;

		for (auto& Widget : Widgets)
		{
			if (Widget && Widget->IsVisible() && Widget->IsFocusable())
			{
				float WidgetDepth = Widget->GetRenderDepth();
				if (WidgetDepth > CurrentDepth && WidgetDepth < BestDepth)
				{
					BestDepth = WidgetDepth;
					BestCandidate = Widget;
				}
			}
		}

		if (!BestCandidate)
		{
			for (auto& Widget : Widgets)
			{
				if (Widget && Widget->IsVisible() && Widget->IsFocusable())
				{
					float WidgetDepth = Widget->GetRenderDepth();
					if (WidgetDepth < BestDepth)
					{
						BestDepth = WidgetDepth;
						BestCandidate = Widget;
					}
				}
			}
		}

		return BestCandidate;
	}

	shared<Widget> GUISubsystem::FindPreviousFocusable() const
	{
		auto Current = FocusedWidget.lock();
		float CurrentDepth = Current ? Current->GetRenderDepth() : FLT_MAX;

		shared<Widget> BestCandidate;
		float BestDepth = -FLT_MAX;
		bool bFoundLower = false;

		for (auto& Widget : Widgets)
		{
			if (Widget && Widget->IsVisible() && Widget->IsFocusable())
			{
				float WidgetDepth = Widget->GetRenderDepth();
				if (WidgetDepth < CurrentDepth && WidgetDepth >= BestDepth)
				{
					BestDepth = WidgetDepth;
					BestCandidate = Widget;
					bFoundLower = true;
				}
			}
		}

		if (!bFoundLower)
		{
			for (auto& Widget : Widgets)
			{
				if (Widget && Widget->IsVisible() && Widget->IsFocusable())
				{
					float WidgetDepth = Widget->GetRenderDepth();
					if (WidgetDepth >= BestDepth)
					{
						BestDepth = WidgetDepth;
						BestCandidate = Widget;
					}
				}
			}
		}

		return BestCandidate;
	}
}
