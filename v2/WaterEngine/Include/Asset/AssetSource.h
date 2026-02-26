// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
	// =============================================================================
	// Asset Source Interface
	// Abstract base for different asset storage backends
	// =============================================================================
	class AssetSource
	{
	public:
		virtual ~AssetSource() = default;
		
		virtual bool Exists(const string& Path) const = 0;
		virtual bool ReadFile(const string& Path, vector<uint8>& OutData) const = 0;
		virtual const string& GetRootPath() const = 0;
	};
}
