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

		if (Background)
		{
			Background->setPosition(GetWorldPosition());
			Background->setScale(GetWorldScale());
			Window.draw(*Background);
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