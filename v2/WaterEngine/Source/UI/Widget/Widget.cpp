// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/Widget/Widget.h"
#include "Utility/Log.h"
#include "Framework/World/RenderTypes.h"
#include "EngineConfig.h"
#include "UI/UIUtility.h"

namespace we
{
	Widget::Widget()
	{
	}

	void Widget::SetAnchorPosition(Anchor ScreenAnchor, Anchor InWidgetAnchor, vec2f Offset)
	{
		TargetAnchor = ScreenAnchor;
		WidgetAnchor = InWidgetAnchor;
		AnchorOffset = Offset;
		bUseAnchors = true;
		MarkDirty();
	}

	void Widget::SetLocalOffset(const vec2f& Offset)
	{
		LocalOffset = Offset;
		MarkDirty();
	}

	void Widget::SetLocalScale(const vec2f& InScale)
	{
		LocalScale = InScale;
		MarkDirty();
	}

	vec2f Widget::GetWorldScale() const
	{
		UpdateCache();
		return CachedWorldScale;
	}

	void Widget::SetSize(const vec2f& InSize)
	{
		Size = InSize;
		MarkDirty();
	}

	void Widget::SetAutoSize(bool bEnabled, float Padding)
	{
		bAutoSize = bEnabled;
		AutoSizePadding = Padding;
		if (bAutoSize)
		{
			CalculateAutoSize();
		}
	}

	void Widget::CalculateAutoSize()
	{
		if (!bAutoSize || Children.empty()) return;

		float MinX = FLT_MAX;
		float MinY = FLT_MAX;
		float MaxX = -FLT_MAX;
		float MaxY = -FLT_MAX;

		for (auto& WeakChild : Children)
		{
			if (auto Child = WeakChild.lock())
			{
				if (!Child->IsVisible()) continue;

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

	vec2f Widget::GetWorldPosition() const
	{
		UpdateCache();
		return CachedWorldPosition;
	}

	vec2f Widget::GetOrigin() const
	{
		UpdateCache();
		return CachedOrigin;
	}

	void Widget::AddChild(shared<Widget> Child, Anchor InTargetAnchor, Anchor InWidgetAnchor, vec2f InOffset)
	{
		if (!Child || Child.get() == this) return;

		Child->SetParent(this);
		Child->SetAnchorPosition(InTargetAnchor, InWidgetAnchor, InOffset);
		
		// Inherit parent's visibility - if parent is hidden, child should be hidden too
		if (!bVisible)
		{
			Child->SetVisible(false);
		}
		
		// Inherit parent's render depth + 1 (render in front of parent)
		Child->SetRenderDepth(GetRenderDepth() + 1.0f);
		
		Children.push_back(Child);

		if (bAutoSize)
		{
			CalculateAutoSize();
		}
	}

	void Widget::RemoveChild(Widget* Child)
	{
		for (auto It = Children.begin(); It != Children.end();)
		{
			if (auto Locked = It->lock(); Locked && Locked.get() == Child)
			{
				Locked->DetachFromParent();
				It = Children.erase(It);
			}
			else
			{
				++It;
			}
		}
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

	bool Widget::Contains(const vec2f& ScreenPoint) const
	{
		vec2f Pos = GetWorldPosition();
		vec2f Scl = GetWorldScale();
		vec2f RealSize = Size.componentWiseMul(Scl);

		// GetWorldPosition() returns top-left position for anchored widgets
		return PointInRect(ScreenPoint, Pos, RealSize);
	}

	shared<Widget> Widget::FindDeepestChildAt(const vec2f& ScreenPoint)
	{
		if (!IsVisible() || !Contains(ScreenPoint))
		{
			return nullptr;
		}

		// Find the topmost child (highest render depth) that contains the point
		// WITHOUT allocating a vector or sorting - O(n) with O(1) memory
		shared<Widget> TopChild = nullptr;
		float MaxDepth = -FLT_MAX;

		for (auto& WeakChild : Children)
		{
			if (auto Child = WeakChild.lock())
			{
				if (!Child->IsVisible())
					continue;

				// Check if child contains point and has higher depth than current max
				if (Child->Contains(ScreenPoint) && Child->GetRenderDepth() > MaxDepth)
				{
					MaxDepth = Child->GetRenderDepth();
					TopChild = Child;
				}
			}
		}

		// If we found a containing child, recurse into it
		if (TopChild)
		{
			if (auto Hit = TopChild->FindDeepestChildAt(ScreenPoint))
			{
				return Hit;
			}
		}

		// No child contains the point, return self
		return shared_from_this();
	}

	void Widget::SetVisible(bool bInVisible)
	{
		bVisible = bInVisible;
		// Propagate visibility to all children
		for (auto& WeakChild : Children)
		{
			if (auto Child = WeakChild.lock())
			{
				Child->SetVisible(bInVisible);
			}
		}
	}

	void Widget::MarkDirty()
	{
		bDirty = true;
		for (auto& WeakChild : Children)
		{
			if (auto Child = WeakChild.lock())
			{
				Child->MarkDirty();
			}
		}
	}

	void Widget::UpdateCache() const
	{
		if (!bDirty) return;

		CachedWorldScale = LocalScale;
		if (Parent)
		{
			CachedWorldScale = CachedWorldScale.componentWiseMul(Parent->GetWorldScale());
		}

		CachedOrigin = GetAnchorPoint(Size.componentWiseMul(CachedWorldScale), WidgetAnchor);

		if (bUseAnchors)
		{
			vec2f ParentSize = Parent ? Parent->GetSize() : vec2f(EC.RenderResolution);
			vec2f ParentPos = Parent ? Parent->GetWorldPosition() : vec2f{ 0, 0 };
			vec2f ParentScale = Parent ? Parent->GetWorldScale() : vec2f{ 1, 1 };

			vec2f ParentPoint = GetAnchorPoint(ParentSize.componentWiseMul(ParentScale), TargetAnchor);
			vec2f MyPoint = GetAnchorPoint(Size.componentWiseMul(CachedWorldScale), WidgetAnchor);

			CachedWorldPosition = ParentPos + ParentPoint - MyPoint + AnchorOffset + LocalOffset;
		}
		else
		{
			vec2f ParentPos = Parent ? Parent->GetWorldPosition() : vec2f{ 0, 0 };
			CachedWorldPosition = ParentPos + LocalOffset;
		}

		bDirty = false;
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
}