// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Interface/IAssetDirector.h"

namespace we
{
    // =========================================================================
    // FileDirectory - Loose file asset directory for Debug mode
    // Reads directly from filesystem (Content/ folder)
    // =========================================================================
    class FileDirectory : public IAssetDirector
    {
    public:
        explicit FileDirectory(const string& RootPath);
        
        bool Exists(stringView Path) const override;
        bool ReadFile(stringView Path, vector<uint8>& OutData) const override;
        
    private:
        string RootPath;
    };
}
