// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/SaveLoadSubsystem.h"
#include "Utility/Log.h"

// Platform-specific includes for user data directory
#ifdef _WIN32
#include <shlobj.h>
#include <windows.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif

namespace we
{
	SaveLoadSubsystem::SaveLoadSubsystem()
	{
		SavePath = GetSavePath();

		if (filestream File{ SavePath })
		{
			try { SaveData = json::parse(File); }
			catch (...) { SaveData = json::object(); }
		}
		else
		{
			SaveData = json::object();
		}
	}

	SaveLoadSubsystem::~SaveLoadSubsystem()
	{
		Save();
	}

	string SaveLoadSubsystem::GetSavePath() const
	{
#ifdef _WIN32
		// Windows: %APPDATA%/WaterEngine/Save.json
		char Path[MAX_PATH];
		if (SUCCEEDED(SHGetFolderPathA(nullptr, CSIDL_APPDATA, nullptr, 0, Path)))
		{
			string Result = Path;
			Result += "\\WaterEngine";
			CreateDirectoryA(Result.c_str(), nullptr); // Ensure dir exists
			return Result + "\\Save.json";
		}
		return "Save.json"; // Fallback
#else
		// Linux/macOS: ~/.config/WaterEngine/Save.json
		const char* Home = getenv("HOME");
		if (!Home)
		{
			struct passwd* Pw = getpwuid(getuid());
			Home = Pw ? Pw->pw_dir : ".";
		}
		string Result = Home;
		Result += "/.config/WaterEngine";

		// Create directory if needed
		string Cmd = "mkdir -p " + Result;
		system(Cmd.c_str());

		return Result + "/Save.json";
#endif
	}

	void SaveLoadSubsystem::Save()
	{
		if (outstream File{ SavePath })
		{
			File << SaveData.dump(4);
			//LOG("Save written to: {}", SavePath);
			LOG("Auto Save Enabled");
		}
		else
		{
			ERROR("Failed to write save to: {}", SavePath);
		}
	}

	bool SaveLoadSubsystem::Has(stringView Key) const
	{
		return SaveData.contains(Key);
	}

	void SaveLoadSubsystem::Erase(stringView Key)
	{
		SaveData.erase(Key);
	}

	void SaveLoadSubsystem::Clear()
	{
		SaveData.clear();
	}
}