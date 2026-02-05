// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/Widget/Panel.h"
#include "Subsystem/RenderSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Subsystem/GUISubsystem.h"
#include "Framework/GameWindow.h"
#include "Utility/WindowUtility.h"

namespace we
{
	Panel::Panel(const string& BackgroundTexturePath)
	{
		if (!BackgroundTexturePath.empty())
		{
			BgTexture = Asset().LoadTexture(BackgroundTexturePath);
			if (BgTexture)
			{
				BgSprite.emplace(*BgTexture);
				Size = vec2f(BgTexture->getSize());
				BgSprite->setOrigin(vec2f(BgTexture->getSize()).componentWiseMul({ 0.5f, 0.5f }));
			}
		}
	}

	void Panel::AddChild(shared<Widget> Child)
	{
		if (Child)
		{
			Children.push_back(std::move(Child));
		}
	}

	void Panel::RemoveChild(Widget* Child)
	{
		for (auto it = Children.begin(); it != Children.end();)
		{
			if (it->get() == Child)
			{
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
			BgSprite->setPosition(Position);
			Window.draw(*BgSprite);
		}

		for (auto& Child : Children)
		{
			if (Child->IsVisible())
			{
				vec2f OriginalPos = Child->GetPosition();
				Child->SetPosition(Position + OriginalPos);
				Child->Render(Window);
				Child->SetPosition(OriginalPos);
			}
		}
	}

	bool Panel::HandleClick(const vec2f& MousePos)
	{
		if (!bVisible) return false;

		for (auto it = Children.rbegin(); it != Children.rend(); ++it)
		{
			if ((*it)->HandleClick(MousePos))
			{
				return true;
			}
		}
		return Contains(MousePos);
	}

	void Panel::OnHover()
	{
		//for (auto& Child : Children)
		//{
		//	if (Child->IsVisible() && Child->Contains(MousePos)) // Need mouse pos
		//	{
		//		Child->OnHover();
		//	}
		//}
	}

	void Panel::OnUnhover()
	{
		for (auto& Child : Children)
		{
			Child->OnUnhover();
		}
	}
}