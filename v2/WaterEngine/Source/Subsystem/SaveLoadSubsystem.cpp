// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/SaveLoadSubsystem.h"

namespace we
{
	SaveLoadSubsystem::SaveLoadSubsystem()
	{
		if (filestream File{ "Content/Save/Save.json" })
		{
			try	{ SaveData = json::parse(File);	}
			catch (...)	{ SaveData = json::object(); }
		}
		else { SaveData = json::object(); }
	}

	SaveLoadSubsystem::~SaveLoadSubsystem()
	{
		Save();
	}

	void SaveLoadSubsystem::Save()
	{
		if (outstream File{ "Content/Save/Save.json" })
		{
			File << SaveData.dump(4);
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