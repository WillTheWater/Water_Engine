// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
	class IAssetDirector
	{
	public:
		virtual ~IAssetDirector() = default;

		virtual bool Exists(stringView Path) const = 0;
		virtual bool ReadFile(stringView Path, vector<uint8>& OutData) const = 0;
	};
}
