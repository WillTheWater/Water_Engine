// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include <filesystem>
#include <physfs.h>

#include "AssetDirectory/PakDirectory.h"
#include "Utility/Log.h"

namespace we
{
	PakDirectory::PakDirectory(const string& PakFilePath)
		: MountedPakPath{ PakFilePath }
	{
		if (!PHYSFS_isInit())
		{
			PHYSFS_init(nullptr);
		}

		PHYSFS_mount(PHYSFS_getBaseDir(), "/", 1);

		string ResolvedPath = PakFilePath;
		if (!std::filesystem::path(PakFilePath).is_absolute())
		{
			ResolvedPath = (std::filesystem::path(PHYSFS_getBaseDir()) / PakFilePath).string();
		}
		MountedPakPath = ResolvedPath;

		if (!PHYSFS_mount(MountedPakPath.c_str(), "/", 1))
		{
			LOG("Failed to mount pak: {}", MountedPakPath);
		}
	}

	PakDirectory::~PakDirectory()
	{
		PHYSFS_unmount(MountedPakPath.c_str());

		if (PHYSFS_isInit())
		{
			PHYSFS_deinit();
		}
	}

	bool PakDirectory::Exists(stringView Path) const
	{
		return PHYSFS_exists(Path.data()) != 0;
	}

	bool PakDirectory::ReadFile(stringView Path, vector<uint8>& OutData) const
	{
		PHYSFS_File* File = PHYSFS_openRead(Path.data());
		if (!File)
		{
			return false;
		}

		const PHYSFS_sint64 Size = PHYSFS_fileLength(File);
		if (Size <= 0)
		{
			PHYSFS_close(File);
			return false;
		}

		OutData.resize(static_cast<size_t>(Size));
		const PHYSFS_sint64 BytesRead = PHYSFS_readBytes(File, OutData.data(), Size);

		PHYSFS_close(File);

		return BytesRead == Size;
	}
}
