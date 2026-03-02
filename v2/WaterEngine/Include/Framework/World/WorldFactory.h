// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
	struct EngineSubsystem;
	class World;

	using WorldFactoryFunc = std::function<shared<World>(EngineSubsystem&)>;

	// =========================================================================
	// WorldFactory - Template-based world registration
	// Game registers world types, Engine uses them by name
	// =========================================================================
	class WorldFactory
	{
	public:
		// Register a world type with factory function
		template<typename WorldType>
		void Register(const string& Name)
		{
			static_assert(std::is_base_of_v<World, WorldType>, "Must derive from World");
			Factories[Name] = [](EngineSubsystem& Sub) {
				return make_shared<WorldType>(Sub);
			};
		}

		// Register with custom setup callback
		template<typename WorldType>
		void Register(const string& Name, std::function<void(WorldType&)> Setup)
		{
			static_assert(std::is_base_of_v<World, WorldType>, "Must derive from World");
			Factories[Name] = [Setup](EngineSubsystem& Sub) -> shared<World> {
				auto world = make_shared<WorldType>(Sub);
				if (Setup) Setup(*world);
				return world;
			};
		}

		// Create world by name (returns nullptr if not registered)
		shared<World> Create(const string& Name, EngineSubsystem& Sub);

		bool IsRegistered(const string& Name) const;

		// Direct template creation (no registration needed)
		template<typename WorldType>
		static shared<World> Create(EngineSubsystem& Sub)
		{
			static_assert(std::is_base_of_v<World, WorldType>, "Must derive from World");
			return make_shared<WorldType>(Sub);
		}

	private:
		dictionary<string, WorldFactoryFunc> Factories;
	};
}
