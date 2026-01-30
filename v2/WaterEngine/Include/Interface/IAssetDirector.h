// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"

namespace we
{
	class IAssetDirector
	{
	public:
		virtual ~IAssetDirector() = default;

		// Returns true if the asset exists
		virtual bool Exists(stringView Path) const = 0;

		// Loads entire file into memory
		virtual bool ReadFile(stringView Path, list<uint8>& OutData) const = 0;
	};
}