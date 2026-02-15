// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Core/JsonTypes.h"


namespace we
{
	class SaveLoadSubsystem
	{
	public:
		SaveLoadSubsystem();
		~SaveLoadSubsystem();

		template <typename T>
		void Set(stringView Key, const T& Value);

		template <typename T>
		T Get(stringView Key, const T& DefaultValue = T{}) const;

		void Save();
		bool Has(stringView Key) const;
		void Erase(stringView Key);
		void Clear();

	private:
		json SaveData;
	};

	template <typename T>
	void SaveLoadSubsystem::Set(stringView Key, const T& Value)
	{
		SaveData[Key] = Value;
	}

	template <typename T>
	T SaveLoadSubsystem::Get(stringView Key, const T& DefaultValue) const
	{
		return SaveData.value(Key, DefaultValue);
	}
}