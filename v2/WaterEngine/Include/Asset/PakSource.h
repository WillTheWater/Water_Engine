// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Asset/AssetSource.h"

namespace we
{
	class PakSource : public AssetSource
	{
	public:
		explicit PakSource(const string& PakFilePath);
		~PakSource();
		
		bool Exists(const string& Path) const override;
		bool ReadFile(const string& Path, vector<uint8>& OutData) const override;
		const string& GetRootPath() const override { return PakPath; }

		static bool MountPak(const string& PakFilePath, const string& MountPoint = "/");

	private:
		string PakPath;
	};
}
