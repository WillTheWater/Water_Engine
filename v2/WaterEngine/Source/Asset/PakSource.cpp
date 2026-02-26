// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Asset/PakSource.h"
#include "Utility/Log.h"
#include <physfs.h>
#include <filesystem>

namespace we
{
	PakSource::PakSource(const string& PakFilePath)
		: PakPath(PakFilePath)
	{
		LOG("PakSource created for: {}", PakFilePath);
	}

	PakSource::~PakSource()
	{
		if (PHYSFS_isInit() && !PakPath.empty())
		{
			PHYSFS_unmount(PakPath.c_str());
		}
	}

	bool PakSource::Exists(const string& Path) const
	{
		return PHYSFS_exists(Path.c_str()) != 0;
	}

	bool PakSource::ReadFile(const string& Path, vector<uint8>& OutData) const
	{
		PHYSFS_File* File = PHYSFS_openRead(Path.c_str());
		if (!File)
		{
			LOG("Failed to open file from pak: {}", Path);
			return false;
		}

		PHYSFS_sint64 Size = PHYSFS_fileLength(File);
		if (Size <= 0)
		{
			PHYSFS_close(File);
			return false;
		}

		OutData.resize(static_cast<size_t>(Size));
		PHYSFS_sint64 Read = PHYSFS_readBytes(File, OutData.data(), Size);
		PHYSFS_close(File);

		return Read == Size;
	}

	bool PakSource::MountPak(const string& PakFilePath, const string& MountPoint)
	{
		LOG("Mounting pak: {} at {}", PakFilePath, MountPoint);

		if (!PHYSFS_isInit())
		{
			if (!PHYSFS_init(nullptr))
			{
				LOG("Failed to initialize PhysicsFS");
				return false;
			}
		}

		PHYSFS_mount(PHYSFS_getBaseDir(), "/", 0);

		string ResolvedPath = PakFilePath;
		if (!std::filesystem::path(PakFilePath).is_absolute())
		{
			ResolvedPath = (std::filesystem::path(PHYSFS_getBaseDir()) / PakFilePath).string();
		}

		if (!PHYSFS_mount(ResolvedPath.c_str(), MountPoint.c_str(), 1))
		{
			LOG("Failed to mount pak: {}", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
			return false;
		}

		LOG("Pak mounted successfully: {} -> {}", ResolvedPath, MountPoint);
		return true;
	}
}
