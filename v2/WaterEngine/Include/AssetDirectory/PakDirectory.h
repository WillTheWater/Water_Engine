// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Interface/IAssetDirector.h"

namespace we
{
	class PakDirectory : public IAssetDirector
	{
	public:
		explicit PakDirectory(const string& PakFilePath);
		~PakDirectory();

		bool Exists(stringView Path) const override;

		bool ReadFile(stringView Path, vector<uint8>& OutData) const override;

	private:
		string MountedPakPath;
	};
}