// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/Widget/Panel.h"
#include "Subsystem/RenderSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Framework/GameWindow.h"

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
				// Store original position
				vec2f OriginalPos = Child->GetPosition();
				// Calculate world position: panel center + child offset
				vec2f WorldPos = Position + OriginalPos;
				Child->SetPosition(WorldPos);
				Child->Render(Window);
				// Restore local position
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
}