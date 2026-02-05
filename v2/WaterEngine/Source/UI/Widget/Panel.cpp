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
	Panel::Panel(EngineSubsystem& Subsystem, const string& BackgroundTexturePath)
		: Widget{ Subsystem }
	{
		if (!BackgroundTexturePath.empty())
		{
			BgTexture = Asset().LoadTexture(BackgroundTexturePath);
			if (BgTexture)
			{
				BgSprite.emplace(*BgTexture);
				Size = vec2f(BgTexture->getSize());
				BgSprite->setOrigin(Size.componentWiseMul({ 0.5f, 0.5f }));
			}
		}
	}

	void Panel::AddChild(shared<Widget> Child)
	{
		if (Child)
		{
			Child->SetParent(this);
			Children.push_back(std::move(Child));
		}
	}

	void Panel::RemoveChild(Widget* Child)
	{
		for (auto it = Children.begin(); it != Children.end();)
		{
			if (it->get() == Child)
			{
				(*it)->SetParent(nullptr);
				it = Children.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	void Panel::ClearChildren()
	{
		for (auto& Child : Children)
		{
			Child->SetParent(nullptr);
		}
		Children.clear();
	}

	void Panel::Update(float DeltaTime)
	{
		for (auto& Child : Children)
		{
			if (Child->IsVisible())
			{
				Child->Update(DeltaTime);
			}
		}
	}

	void Panel::Render(GameWindow& Window)
	{
		if (!bVisible) return;

		if (BgSprite)
		{
			BgSprite->setPosition(GetWorldPosition());
			BgSprite->setRotation(GetWorldRotation());
			BgSprite->setScale(GetWorldScale());
			Window.draw(*BgSprite);
		}

		for (auto& Child : Children)
		{
			if (Child->IsVisible())
			{
				Child->Render(Window);
			}
		}
	}
}