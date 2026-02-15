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

		// Returns true if the asset exists
		virtual bool Exists(stringView Path) const = 0;

		// Loads entire file into memory
		virtual bool ReadFile(stringView Path, vector<uint8>& OutData) const = 0;
	};
}