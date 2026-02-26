// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Asset/DirectorySource.h"
#include <filesystem>
#include <fstream>
#include "Utility/Log.h"

namespace we
{
	DirectorySource::DirectorySource(const string& DirectoryPath)
		: RootPath(DirectoryPath)
	{
	}

	bool DirectorySource::Exists(const string& Path) const
	{
		std::filesystem::path fullPath = std::filesystem::path(RootPath) / Path;
		return std::filesystem::exists(fullPath);
	}

	bool DirectorySource::ReadFile(const string& Path, vector<uint8>& OutData) const
	{
		std::filesystem::path fullPath = std::filesystem::path(RootPath) / Path;
		
		std::ifstream file(fullPath, std::ios::binary | std::ios::ate);
		if (!file.is_open())
		{
			LOG("Failed to open file: {}", fullPath.string());
			return false;
		}

		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);

		OutData.resize(size);
		if (!file.read(reinterpret_cast<char*>(OutData.data()), size))
		{
			LOG("Failed to read file: {}", fullPath.string());
			return false;
		}

		return true;
	}
}
