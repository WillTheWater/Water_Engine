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
		ScreenAnchor = InScreenAnchor;
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
		vec2f Org = GetOrigin().componentWiseMul(Scl);
		vec2f RealSize = Size.componentWiseMul(Scl);

		return WorldPoint.x >= Pos.x - Org.x &&
			WorldPoint.x <= Pos.x - Org.x + RealSize.x &&
			WorldPoint.y >= Pos.y - Org.y &&
			WorldPoint.y <= Pos.y - Org.y + RealSize.y;
	}

	void Widget::MarkDirty()
	{
		bDirty = true;
	}

	void Widget::UpdateCache() const
	{
		if (!bDirty) return;

		CachedOrigin = GetAnchorPoint(Size, WidgetAnchor);

		if (Parent)
		{
			CachedWorldScale = Parent->GetWorldScale().componentWiseMul(LocalScale);
		}
		else
		{
			CachedWorldScale = LocalScale;
		}

		if (bUseAnchors && !Parent)
		{
			CachedWorldPosition = CalculateAnchorPosition();
		}
		else if (Parent)
		{
			CachedWorldPosition = Parent->GetWorldPosition() + LocalOffset;
		}
		else
		{
			CachedWorldPosition = LocalOffset;
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

	vec2f Widget::CalculateAnchorPosition() const
	{
		vec2f ScreenSize = Subsystem.Render->GetRenderSize();

		vec2f ScreenPoint = GetAnchorPoint(ScreenSize, ScreenAnchor);
		vec2f WidgetOffset = GetAnchorPoint(Size.componentWiseMul(LocalScale), WidgetAnchor);

		return ScreenPoint - WidgetOffset + AnchorOffset;
	}
}