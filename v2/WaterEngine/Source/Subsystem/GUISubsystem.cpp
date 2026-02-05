// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/GUISubsystem.h"
#include "Framework/GameWindow.h"
#include "UI/Widget/Widget.h"

namespace we
{
	GUISubsystem::GUISubsystem(GameWindow& Window)
		: Window{ Window }
	{
	}

	GUISubsystem::~GUISubsystem()
	{
		Clear();
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
		for (auto& Widget : Widgets)
		{
			if (Widget->IsVisible())
			{
				Widget->Render(Window);
			}
		}
	}

	void GUISubsystem::AddWidget(shared<Widget> InWidget)
	{
		if (InWidget)
		{
			Widgets.push_back(std::move(InWidget));
		}
	}

	void GUISubsystem::RemoveWidget(Widget* InWidget)
	{
		auto it = std::remove_if(Widgets.begin(), Widgets.end(),
			[InWidget](const shared<Widget>& W)
			{
				return W.get() == InWidget;
			});

		Widgets.erase(it, Widgets.end());
	}

	void GUISubsystem::Clear()
	{
		Widgets.clear();
	}
}