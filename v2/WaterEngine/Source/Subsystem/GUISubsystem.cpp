// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/GUISubsystem.h"
#include "Subsystem/RenderSubsystem.h"
#include "Subsystem/CursorSubsystem.h"
#include "Framework/EngineSubsystem.h"
#include "UI/Widget/Widget.h"
#include "Framework/World/RenderTypes.h"
#include "Utility/Log.h"

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
		// Check if we should consume this event
		bool bConsumed = false;

		// Mouse events
		if (auto MouseMoved = Event.getIf<sf::Event::MouseMoved>())
		{
			HandleMouseMoved(*MouseMoved);
			bConsumed = bHasMouseFocus;  // Consume if hovering UI
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
				bConsumed = false;  // Always let release through to prevent stuck states
			}
		}
		// Keyboard navigation
		else if (auto KeyPressed = Event.getIf<sf::Event::KeyPressed>())
		{
			HandleKeyPressed(*KeyPressed);
			// Consume navigation keys if we have focus
			bConsumed = (FocusedWidget.lock() != nullptr) &&
				(KeyPressed->scancode == sf::Keyboard::Scan::Tab ||
					KeyPressed->scancode == sf::Keyboard::Scan::Enter ||
					KeyPressed->scancode == sf::Keyboard::Scan::Escape);
		}
		// Gamepad navigation
		else if (auto JoystickPressed = Event.getIf<sf::Event::JoystickButtonPressed>())
		{
			HandleJoystickPressed(*JoystickPressed);
			bConsumed = (FocusedWidget.lock() != nullptr);
		}
		// Text input (for text fields)
		else if (auto TextEntered = Event.getIf<sf::Event::TextEntered>())
		{
			HandleTextEntered(*TextEntered);
			bConsumed = (FocusedWidget.lock() != nullptr);
		}

		return bConsumed;
	}

	void GUISubsystem::HandleMouseMoved(const sf::Event::MouseMoved& Mouse)
	{
		vec2f MousePos = Subsystem.Cursor->GetPosition();
		UpdateHoverState(MousePos);
	}

	void GUISubsystem::HandleMousePressed(const sf::Event::MouseButtonPressed& Btn)
	{
		vec2f MousePos = Subsystem.Cursor->GetPosition();
		auto Target = FindWidgetAt(MousePos);

		if (Target && Target->IsVisible())
		{
			// Set focus to clicked widget
			if (auto OldFocus = FocusedWidget.lock())
			{
				OldFocus->OnFocusLost.Broadcast();
			}

			FocusedWidget = Target;
			Target->OnFocusGained.Broadcast();

			PressedWidget = Target;
			Target->OnClicked.Broadcast();
		}
		else
		{
			// Clicked on empty space, clear focus
			if (auto OldFocus = FocusedWidget.lock())
			{
				OldFocus->OnFocusLost.Broadcast();
				FocusedWidget.reset();
			}
		}
	}

	void GUISubsystem::HandleMouseReleased(const sf::Event::MouseButtonReleased& Btn)
	{
		PressedWidget.reset();
	}

	void GUISubsystem::HandleKeyPressed(const sf::Event::KeyPressed& Key)
	{
		// Tab navigation
		if (Key.scancode == sf::Keyboard::Scan::Tab)
		{
			if (Key.shift)
				NavigatePrevious();
			else
				NavigateNext();
		}
		// Confirm
		else if (Key.scancode == sf::Keyboard::Scan::Enter || Key.scancode == sf::Keyboard::Scan::Space)
		{
			ActivateFocused();
		}
		// Cancel
		else if (Key.scancode == sf::Keyboard::Scan::Escape)
		{
			// TODO: Close top modal or pause menu
		}
	}

	void GUISubsystem::HandleJoystickPressed(const sf::Event::JoystickButtonPressed& Btn)
	{
		// Map common gamepad buttons to GUI actions
		// South button (A on Xbox, X on PlayStation) = Confirm
		// East button (B on Xbox, Circle on PlayStation) = Cancel
		// LB/RB or D-pad for navigation

		// TODO: Map based on your Input::LogicToHardware if available
		// For now, assume standard mappings
		if (Btn.button == 0) // South/Confirm
		{
			ActivateFocused();
		}
		else if (Btn.button == 1) // East/Cancel
		{
			// Cancel action
		}
		else if (Btn.button == 4) // LB
		{
			NavigatePrevious();
		}
		else if (Btn.button == 5) // RB
		{
			NavigateNext();
		}
	}

	void GUISubsystem::HandleTextEntered(const sf::Event::TextEntered& Text)
	{
		// Pass to focused widget if it's a text input
		if (auto Focused = FocusedWidget.lock())
		{
			// TODO: Widget should handle text input
			// Focused->OnTextEntered.Broadcast(Text.unicode);
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
		// Collect all visible widget drawables using RenderDepth for sorting
		static vector<RenderDepth> GUIDepths;
		GUIDepths.clear();

		for (auto& Widget : Widgets)
		{
			if (Widget && Widget->IsVisible())
			{
				Widget->CollectRenderDepths(GUIDepths);
			}
		}

		// Sort by render depth (back to front - lower values drawn first)
		std::sort(GUIDepths.begin(), GUIDepths.end());

		// Batch render all UI to UI layer
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
				OldHovered->OnFocusLost.Broadcast(); // Or specific OnHoverLost
			}
			if (NewHovered)
			{
				NewHovered->OnFocusGained.Broadcast(); // Or specific OnHoverGained
			}

			HoveredWidget = NewHovered;
			bHasMouseFocus = (NewHovered != nullptr);
		}
	}

	shared<Widget> GUISubsystem::FindWidgetAt(const vec2f& ScreenPoint) const
	{
		// Search front-to-back (highest render depth first)
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
		// Simple implementation: find next widget with higher render depth that is focusable
		// TODO: Sort by position for spatial navigation (left-to-right, top-to-bottom)

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

		// Wrap around to first if none found
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
		// Inverse of FindNextFocusable
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

		// Wrap around to last if none found
		if (!bFoundLower)
		{
			for (auto& Widget : Widgets)
			{
				if (Widget && Widget->IsVisible() && Widget->IsFocusable())
				{
					int WidgetDepth = Widget->GetRenderDepth();
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