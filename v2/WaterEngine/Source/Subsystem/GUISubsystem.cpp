// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/GUISubsystem.h"
#include "Framework/GameWindow.h"
#include "Input/InputBinding.h"
#include "Subsystem/ResourceSubsystem.h"
#include "EngineConfig.h"
#include "UI/Widget/Widget.h"
#include "UI/Cursor/Cursor.h"
#include "Subsystem/RenderSubsystem.h"

namespace we
{
	GUISubsystem::GUISubsystem(GameWindow& InWindow)
		: Window{ InWindow }
	{
	}

	GUISubsystem::~GUISubsystem()
	{
		Clear();
	}

	vec2f GUISubsystem::GetMousePosition() const
	{
		return vec2f{0,0};
	}

	void GUISubsystem::Update(float DeltaTime)
	{
		vec2f MousePos = GetMousePosition();

		// Update hover states
		Widget* NewHovered = nullptr;
		for (auto& Widget : Widgets)
		{
			if (Widget->IsVisible() && Widget->Contains(MousePos))
			{
				NewHovered = Widget.get();
				break; // Topmost only
			}
		}

		if (NewHovered != HoveredWidget)
		{
			if (HoveredWidget) HoveredWidget->OnUnhover();
			if (NewHovered) NewHovered->OnHover();
			HoveredWidget = NewHovered;
		}

		// Update all widgets
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
		for (auto& Widget : Widgets)
		{
			if (Widget->IsVisible())
			{
				Widget->Render(Window);
			}
		}
	}

	void GUISubsystem::HandleMousePress()
	{
		vec2f MousePos = GetMousePosition();

		for (auto& Widget : Widgets)
		{
			if (Widget->IsVisible() && Widget->Contains(MousePos))
			{
				PressedWidget = Widget.get();
				PressedWidget->OnPress();
				Widget->HandleClick(MousePos);
				return;
			}
		}
	}

	void GUISubsystem::HandleMouseRelease()
	{
		if (PressedWidget)
		{
			PressedWidget->OnRelease();
			PressedWidget = nullptr;
		}
	}

	void GUISubsystem::AddWidget(shared<Widget> InWidget)
	{
		Widgets.push_back(std::move(InWidget));
	}

	void GUISubsystem::RemoveWidget(Widget* InWidget)
	{
		auto it = std::remove_if(Widgets.begin(), Widgets.end(),
			[InWidget](const shared<Widget>& W)
			{
				return W.get() == InWidget;
			});

		Widgets.erase(it, Widgets.end());

		if (HoveredWidget == InWidget) HoveredWidget = nullptr;
		if (PressedWidget == InWidget) PressedWidget = nullptr;
	}

	void GUISubsystem::Clear()
	{
		Widgets.clear();
		HoveredWidget = nullptr;
		PressedWidget = nullptr;
	}
}