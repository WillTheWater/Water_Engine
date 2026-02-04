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
#include "Subsystem/RenderSubsystem.h"

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

	bool GUISubsystem::HandleMousePress(const vec2f& MousePos)
	{
		for (auto& Widget : Widgets)
		{
			if (Widget->IsVisible() && Widget->Contains(MousePos))
			{
				PressedWidget = Widget.get();
				return Widget->HandleClick(MousePos);
			}
		}
		return false;
	}

	void GUISubsystem::HandleMouseRelease()
	{
		PressedWidget = nullptr;
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

		if (PressedWidget == InWidget)
		{
			PressedWidget = nullptr;
		}
	}

	void GUISubsystem::Clear()
	{
		Widgets.clear();
		PressedWidget = nullptr;
	}
}