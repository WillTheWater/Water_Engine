// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Asset/AssetSource.h"

namespace we
{
	// =============================================================================
	// Directory Asset Source
	// Reads assets directly from filesystem (for debug/development)
	// =============================================================================
	class DirectorySource : public AssetSource
	{
	public:
		explicit DirectorySource(const string& DirectoryPath);
		
		bool Exists(const string& Path) const override;
		bool ReadFile(const string& Path, vector<uint8>& OutData) const override;
		const string& GetRootPath() const override { return RootPath; }

	private:
		string RootPath;
	};
}
