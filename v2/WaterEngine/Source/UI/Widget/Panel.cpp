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
			Children.push_back(weak<Widget>(Child));
		}
	}

	void Panel::RemoveChild(Widget* Child)
	{
		for (auto it = Children.begin(); it != Children.end();)
		{
			auto Locked = it->lock();
			if (Locked && Locked.get() == Child)
			{
				Locked->DetachFromParent();
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
		for (auto& WeakChild : Children)
		{
			if (auto Child = WeakChild.lock())
			{
				Child->DetachFromParent();
			}
		}
		Children.clear();
	}

	void Panel::Update(float DeltaTime)
	{
		auto newEnd = std::remove_if(Children.begin(), Children.end(),
			[](const weak<Widget>& W) { return W.expired(); });
		Children.erase(newEnd, Children.end());

		for (auto& WeakChild : Children)
		{
			if (auto Child = WeakChild.lock(); Child && Child->IsVisible())
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

		for (auto& WeakChild : Children)
		{
			if (auto Child = WeakChild.lock(); Child && Child->IsVisible())
			{
				Child->Render(Window);
			}
		}
	}
}