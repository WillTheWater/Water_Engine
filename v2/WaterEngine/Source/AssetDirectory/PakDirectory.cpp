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
		// Ensure PhysicsFS is initialized before performing any filesystem operations.
		// Initialization is global and must occur once per process.
		if (!PHYSFS_isInit())
		{
			PHYSFS_init(nullptr);
		}

		// Mount the pak archive at the virtual root ("/").
		// All asset paths will be resolved relative to this mount point.
		// The final parameter enables higher mount priority over existing mounts.
		PHYSFS_mount(PHYSFS_getBaseDir(), "/", 1);

		// Resolve relative paths against the executable directory
		std::string ResolvedPath = PakFilePath;
		if (!std::filesystem::path(PakFilePath).is_absolute())
		{
			ResolvedPath = (std::filesystem::path(PHYSFS_getBaseDir()) / PakFilePath).string();
		}
		MountedPakPath = ResolvedPath;

		if (!PHYSFS_mount(MountedPakPath.c_str(), "/", 1))
		{
			LOG("Failed to find .pak")
		}
	}

	PakDirectory::~PakDirectory()
	{
		// Unmount the pak archive to release the virtual filesystem mapping.
		PHYSFS_unmount(MountedPakPath.c_str());

		// Deinitialize PhysicsFS if it is still active.
		// In larger systems, this may be deferred to centralized engine shutdown.
		if (PHYSFS_isInit()) { PHYSFS_deinit();	}
	}

	bool PakDirectory::Exists(stringView Path) const
	{
		// Query PhysicsFS virtual filesystem for existence of the asset path.
		// The path is expected to be relative to the mount point.
		return PHYSFS_exists(Path.data()) != 0;
	}

	bool PakDirectory::ReadFile(stringView Path, vector<uint8>& OutData) const
	{
		// Open the file for reading from the virtual filesystem.
		PHYSFS_File* File = PHYSFS_openRead(Path.data());
		if (!File) { return false; }

		// Retrieve the total size of the file in bytes.
		// A non-positive size indicates an invalid or empty file.
		const PHYSFS_sint64 Size = PHYSFS_fileLength(File);
		if (Size <= 0)
		{
			PHYSFS_close(File);
			return false;
		}

		// Resize the output buffer to hold the entire file contents.
		OutData.resize(static_cast<size_t>(Size));

		// Read the complete file contents into the output buffer.
		const PHYSFS_sint64 bytesRead =	PHYSFS_readBytes(File, OutData.data(), Size);

		// Close the file handle to free internal resources.
		PHYSFS_close(File);

		// Successful read requires the full file size to be read.
		return bytesRead == Size;
	}
}