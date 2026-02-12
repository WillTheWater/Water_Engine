// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/Widget/Panel.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Subsystem/GUISubsystem.h"
#include "Framework/GameWindow.h"
#include "Framework/EngineSubsystem.h"

namespace we
{
	Panel::Panel(EngineSubsystem& Subsystem, const string& TexturePath)
		: Widget{ Subsystem }
	{
		if (TexturePath.empty()) return;

		auto Tex = Asset().LoadTexture(TexturePath);
		if (!Tex) return;

		vec2f TexSize = vec2f(Tex->getSize());
		SetSize(TexSize);

		Background.emplace();
		Background->setSize(TexSize);
		Background->setTexture(&*Tex);
	}

	Panel::Panel(EngineSubsystem& Subsystem, const vec2f& InSize, color FillColor, color OutlineColor, float OutlineThickness)
		: Widget{ Subsystem }
	{
		SetSize(InSize);

		Background.emplace();
		Background->setSize(InSize);
		Background->setFillColor(FillColor);
		Background->setOutlineColor(OutlineColor);
		Background->setOutlineThickness(OutlineThickness);
	}

	void Panel::Update(float DeltaTime)
	{
	}

	void Panel::Render(GameWindow& Window)
	{
		if (!IsVisible()) return;

		if (Background)
		{
			Background->setPosition(GetWorldPosition());
			Background->setScale(GetWorldScale());
			Window.draw(*Background);
		}

		Widget::Render(Window);
	}
}