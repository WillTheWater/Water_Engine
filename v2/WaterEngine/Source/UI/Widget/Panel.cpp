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
	Panel::Panel(EngineSubsystem& Subsystem, const string& BackgroundTexturePath) : Widget{ Subsystem }
	{
		if (BackgroundTexturePath.empty()) return;
		BgTexture = Asset().LoadTexture(BackgroundTexturePath);
		if (!BgTexture) return;
		BgSprite.emplace(*BgTexture);
		Size = vec2f(BgTexture->getSize());
		BgSprite->setOrigin(Size * 0.5f);
	}

	void Panel::AddChild(shared<Widget> Child)
	{
		if (!Child) return;
		Child->SetParent(this);
		Children.push_back(weak<Widget>(Child));
	}

	void Panel::RemoveChild(Widget* Child)
	{
		for (auto it = Children.begin(); it != Children.end();)
		{
			if (auto Locked = it->lock(); Locked && Locked.get() == Child)
			{
				Locked->DetachFromParent();
				it = Children.erase(it);
			}
			else ++it;
		}
	}

	void Panel::ClearChildren()
	{
		for (auto& WeakChild : Children)
			if (auto Child = WeakChild.lock()) Child->DetachFromParent();
		Children.clear();
	}

	void Panel::Update(float DeltaTime)
	{
		Children.erase(std::remove_if(Children.begin(), Children.end(),
			[](const weak<Widget>& W) { return W.expired(); }), Children.end());

		for (auto& WeakChild : Children)
			if (auto Child = WeakChild.lock(); Child->IsVisible())
				Child->Update(DeltaTime);
	}

	void Panel::Render(GameWindow& Window)
	{
		if (!IsVisible()) return;
		if (BgSprite)
		{
			BgSprite->setPosition(GetWorldPosition());
			BgSprite->setScale(GetWorldScale());
			Window.draw(*BgSprite);
		}
		for (auto& WeakChild : Children)
			if (auto Child = WeakChild.lock(); Child->IsVisible())
				Child->Render(Window);
	}

	shared<Widget> Panel::FindChildAt(const vec2f& WorldPoint) const
	{
		if (!IsVisible()) return nullptr;
		for (auto it = Children.rbegin(); it != Children.rend(); ++it)
			if (auto Child = it->lock(); Child && Child->IsVisible() && Child->Contains(WorldPoint))
				return Child;
		return nullptr;
	}

	void Panel::OnMouseEnter() {}
	void Panel::OnMouseLeave() { if (auto Child = HoveredChild.lock()) Child->OnMouseLeave(); HoveredChild.reset(); }

	void Panel::OnMouseMoved(const vec2f& MousePos)
	{
		if (!IsVisible()) return;
		auto NewHovered = FindChildAt(MousePos);
		auto OldHovered = HoveredChild.lock();
		if (NewHovered != OldHovered)
		{
			if (OldHovered) OldHovered->OnMouseLeave();
			if (NewHovered) NewHovered->OnMouseEnter();
			HoveredChild = NewHovered;
		}
		if (NewHovered) NewHovered->OnMouseMoved(MousePos);
	}

	bool Panel::OnMouseButtonPressed()
	{
		if (auto Child = HoveredChild.lock()) return Child->OnMouseButtonPressed();
		return false;
	}

	void Panel::OnMouseButtonReleased()
	{
		if (auto Child = HoveredChild.lock()) Child->OnMouseButtonReleased();
	}
}