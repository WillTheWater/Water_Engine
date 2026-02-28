// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "AssetDirectory/FileDirectory.h"
#include "Utility/Log.h"
#include <fstream>

namespace we
{
    FileDirectory::FileDirectory(const string& InRootPath)
        : RootPath(InRootPath)
    {
        LOG("FileDirectory mounted at: {}", RootPath);
    }

    bool FileDirectory::Exists(stringView Path) const
    {
        string FullPath = RootPath + "/" + string(Path);
        std::ifstream file(FullPath);
        return file.good();
    }

    bool FileDirectory::ReadFile(stringView Path, vector<uint8>& OutData) const
    {
        string FullPath = RootPath + "/" + string(Path);
        
        std::ifstream file(FullPath, std::ios::binary | std::ios::ate);
        if (!file.is_open())
        {
            return false;
        }

        std::streamsize size = file.tellg();
        if (size <= 0)
        {
            return false;
        }

        file.seekg(0, std::ios::beg);
        OutData.resize(static_cast<size_t>(size));
        
        if (!file.read(reinterpret_cast<char*>(OutData.data()), size))
        {
            return false;
        }

        return true;
    }
}
