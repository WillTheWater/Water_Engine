// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/Widget/Widget.h"
#include "Framework/EngineSubsystem.h"
#include "UI/Cursor/CursorSubsystem.h"
#include "Utility/WindowUtility.h"
#include "Utility/Log.h"

namespace we
{
	Widget::Widget(EngineSubsystem& InSubsystem)
		: Subsystem{ InSubsystem }
	{
	}

	void Widget::SetAnchorPosition(Anchor InScreenAnchor, Anchor InWidgetAnchor, vec2f Offset)
	{
		TargetAnchor = InScreenAnchor;
		WidgetAnchor = InWidgetAnchor;
		AnchorOffset = Offset;
		bUseAnchors = true;
		MarkDirty();
	}

	void Widget::AddChild(shared<Widget> Child, Anchor InTargetAnchor, Anchor InWidgetAnchor, vec2f InOffset)
{
    if (!Child || Child.get() == this) return;

    Child->SetParent(this);
    
    if (bAutoSize)
    {
        // Force TopLeft target anchor
        Child->SetAnchorPosition(Anchor::TopLeft, InWidgetAnchor, InOffset);
    }
    else
    {
        Child->SetAnchorPosition(InTargetAnchor, InWidgetAnchor, InOffset);
    }

    Children.push_back(Child);
    
    if (bAutoSize)
        CalculateAutoSize();
}

	void Widget::SetAutoSize(bool bEnabled, float Padding)
	{
		bAutoSize = bEnabled;
		AutoSizePadding = Padding;
		if (bAutoSize)
			CalculateAutoSize();
	}

	void Widget::CalculateAutoSize()
	{
		if (!bAutoSize || Children.empty()) return;

		float MinX = FLT_MAX;
		float MinY = FLT_MAX;
		float MaxX = -FLT_MAX;
		float MaxY = -FLT_MAX;

		for (auto& Weak : Children)
		{
			if (auto Child = Weak.lock())
			{
				if (!Child->IsVisible()) continue;

				// Child local position = AnchorOffset + LocalOffset
				// In auto-size mode, TargetAnchor is forced to TopLeft so this is correct
				vec2f ChildPos = Child->AnchorOffset + Child->LocalOffset;
				vec2f ChildSize = Child->GetSize().componentWiseMul(Child->GetWorldScale());
				vec2f ChildOrigin = Child->GetOrigin().componentWiseMul(Child->GetWorldScale());

				float Left = ChildPos.x - ChildOrigin.x;
				float Top = ChildPos.y - ChildOrigin.y;
				float Right = Left + ChildSize.x;
				float Bottom = Top + ChildSize.y;

				MinX = std::min(MinX, Left);
				MinY = std::min(MinY, Top);
				MaxX = std::max(MaxX, Right);
				MaxY = std::max(MaxY, Bottom);
			}
		}

		if (MinX != FLT_MAX)
		{
			SetSize({ MaxX - MinX + AutoSizePadding * 2, MaxY - MinY + AutoSizePadding * 2 });
		}
	}

	void Widget::Render(GameWindow& Window)
	{
		RenderChildren(Window);
	}

	void Widget::RenderChildren(GameWindow& Window)
	{
		vector<shared<Widget>> ValidChildren;
		for (auto& ChildWeak : Children)
		{
			if (auto Child = ChildWeak.lock())
				ValidChildren.push_back(Child);
		}

		// Sort by ZOrder
		std::sort(ValidChildren.begin(), ValidChildren.end(),
			[](const shared<Widget>& A, const shared<Widget>& B) {
				return A->GetZOrder() < B->GetZOrder();
			});

		for (auto& Child : ValidChildren)
		{
			if (Child->IsVisible())
				Child->Render(Window);
		}
	}

	void Widget::RemoveChild(Widget* Child)
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

	void Widget::SetLocalOffset(const vec2f& Offset)
	{
		LocalOffset = Offset;
		MarkDirty();
	}

	vec2f Widget::GetLocalOffset() const
	{
		return LocalOffset;
	}

	void Widget::SetLocalScale(const vec2f& InScale)
	{
		LocalScale = InScale;
		MarkDirty();
	}

	vec2f Widget::GetLocalScale() const
	{
		return LocalScale;
	}

	void Widget::SetSize(const vec2f& InSize)
	{
		Size = InSize;
		MarkDirty();
	}

	vec2f Widget::GetSize() const
	{
		return Size;
	}

	vec2f Widget::GetWorldPosition() const
	{
		UpdateCache();
		return CachedWorldPosition;
	}

	vec2f Widget::GetWorldScale() const
	{
		UpdateCache();
		return CachedWorldScale;
	}

	vec2f Widget::GetOrigin() const
	{
		UpdateCache();
		return CachedOrigin;
	}

	void Widget::SetParent(Widget* InParent)
	{
		Parent = InParent;
		MarkDirty();
	}

	void Widget::DetachFromParent()
	{
		Parent = nullptr;
		MarkDirty();
	}

	Widget* Widget::GetParent() const
	{
		return Parent;
	}

	void Widget::SetVisible(bool bInVisible)
	{
		bVisible = bInVisible;
	}

	bool Widget::IsVisible() const
	{
		return bVisible;
	}

	uint Widget::GetZOrder() const
	{
		return ZOrder;
	}

	void Widget::SetZOrder(uint InZOrder)
	{
		ZOrder = InZOrder;
	}

	bool Widget::Contains(const vec2f& WorldPoint) const
	{
		vec2f Pos = GetWorldPosition();
		vec2f Scl = GetWorldScale();

		vec2f RealSize = Size.componentWiseMul(Scl);

		return WorldPoint.x >= Pos.x &&
			WorldPoint.x <= Pos.x + RealSize.x &&
			WorldPoint.y >= Pos.y &&
			WorldPoint.y <= Pos.y + RealSize.y;
	}

	void Widget::MarkDirty()
	{
		bDirty = true;
	}

	void Widget::UpdateCache() const
	{
		/*if (!bDirty) return;

		CachedWorldScale = LocalScale;

		CachedOrigin = GetAnchorPoint(Size, WidgetAnchor);

		if (bUseAnchors)
		{
			vec2f RefSize = Parent ? Parent->GetSize() : Subsystem.Render->GetRenderSize();
			vec2f RefPos = Parent ? Parent->GetWorldPosition() : vec2f{ 0.f, 0.f };

			vec2f ParentScl = Parent ? Parent->GetWorldScale() : vec2f{ 1.f, 1.f };

			vec2f ParentPoint = GetAnchorPoint(RefSize.componentWiseMul(ParentScl), TargetAnchor);

			vec2f MyPoint = GetAnchorPoint(Size.componentWiseMul(CachedWorldScale), WidgetAnchor);

			CachedWorldPosition = RefPos + ParentPoint - MyPoint + AnchorOffset + LocalOffset;
		}
		else
		{
			vec2f RefPos = Parent ? Parent->GetWorldPosition() : vec2f{ 0.f, 0.f };
			CachedWorldPosition = RefPos + LocalOffset;
		}

		bDirty = false;*/
	}

	shared<Widget> Widget::FindDeepestChildAt(const vec2f& WorldPoint)
	{
		if (!IsVisible() || !Contains(WorldPoint)) return nullptr;

		vector<shared<Widget>> ValidChildren;
		for (auto& WeakChild : Children)
		{
			if (auto Child = WeakChild.lock())
				ValidChildren.push_back(Child);
		}

		std::sort(ValidChildren.begin(), ValidChildren.end(), [](const shared<Widget>& A, const shared<Widget>& B) {
			return A->GetZOrder() > B->GetZOrder();
			});

		for (auto& Child : ValidChildren)
		{
			if (auto Hit = Child->FindDeepestChildAt(WorldPoint))
				return Hit;
		}

		return shared_from_this();
	}

	vec2f Widget::GetAnchorPoint(const vec2f& InSize, Anchor InAnchor)
	{
		switch (InAnchor)
		{
		case Anchor::TopLeft:      return { 0, 0 };
		case Anchor::TopCenter:    return { InSize.x * 0.5f, 0 };
		case Anchor::TopRight:     return { InSize.x, 0 };
		case Anchor::CenterLeft:   return { 0, InSize.y * 0.5f };
		case Anchor::Center:       return { InSize.x * 0.5f, InSize.y * 0.5f };
		case Anchor::CenterRight:  return { InSize.x, InSize.y * 0.5f };
		case Anchor::BottomLeft:   return { 0, InSize.y };
		case Anchor::BottomCenter: return { InSize.x * 0.5f, InSize.y };
		case Anchor::BottomRight:  return { InSize.x, InSize.y };
		}
		return { 0, 0 };
	}

	vec2f Widget::CalculateAnchorPosition() const
	{
		/*vec2f ScreenSize = Subsystem.Render->GetRenderSize();

		vec2f ScreenPoint = GetAnchorPoint(ScreenSize, TargetAnchor);
		vec2f WidgetOffset = GetAnchorPoint(Size.componentWiseMul(LocalScale), WidgetAnchor);

		return ScreenPoint - WidgetOffset + AnchorOffset;*/
		return vec2f{};
	}
}