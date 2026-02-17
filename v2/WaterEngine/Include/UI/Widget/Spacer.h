// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "UI/Widget/Widget.h"

namespace we
{
	class Spacer : public Widget
	{
	public:
		Spacer(float Width = 10.f, float Height = 10.f)
		{
			SetSize({ Width, Height });
		}
	};
}
