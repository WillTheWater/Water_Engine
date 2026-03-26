// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Core/JsonTypes.h"

namespace we
{
	class SaveSubsystem
	{
	public:
		SaveSubsystem();
		~SaveSubsystem();

		template <typename T>
		void Set(stringView Key, const T& Value);

		template <typename T>
		T Get(stringView Key, const T& DefaultValue = T{}) const;

		void Save();
		bool Has(stringView Key) const;
		void Erase(stringView Key);
		void Clear();

	private:
		string GetSavePath() const;

	private:
		json SaveData;
		string SavePath;
	};

	template <typename T>
	void SaveSubsystem::Set(stringView Key, const T& Value)
	{
		SaveData[Key] = Value;
	}

	template <typename T>
	T SaveSubsystem::Get(stringView Key, const T& DefaultValue) const
	{
		return SaveData.value(Key, DefaultValue);
	}

	// JSON serialization for vec2f
	inline void to_json(json& J, const vec2f& V)
	{
		J = json::array({V.x, V.y});
	}

	inline void from_json(const json& J, vec2f& V)
	{
		V.x = J[0];
		V.y = J[1];
	}

	// JSON serialization for vec2i
	inline void to_json(json& J, const vec2i& V)
	{
		J = json::array({V.x, V.y});
	}

	inline void from_json(const json& J, vec2i& V)
	{
		V.x = J[0];
		V.y = J[1];
	}
}
