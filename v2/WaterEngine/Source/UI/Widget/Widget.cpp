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
	Widget::Widget(EngineSubsystem& InSubsystem) : Subsystem{ InSubsystem } {}

	void Widget::UpdateCache() const
	{
		if (!bDirty) return;
		if (Parent)
		{
			CachedWorldPosition = Parent->GetWorldPosition() + LocalOffset;
			CachedWorldScale = Parent->GetWorldScale().componentWiseMul(LocalScale);
		}
		else
		{
			CachedWorldPosition = LocalOffset;
			CachedWorldScale = LocalScale;
		}
		bDirty = false;
	}

	vec2f Widget::GetWorldPosition() const { UpdateCache(); return CachedWorldPosition; }
	vec2f Widget::GetWorldScale() const { UpdateCache(); return CachedWorldScale; }

	bool Widget::Contains(const vec2f& WorldPoint) const
	{
		return we::Contains(WorldPoint, GetWorldPosition(), Size.componentWiseMul(GetWorldScale()));
	}
}