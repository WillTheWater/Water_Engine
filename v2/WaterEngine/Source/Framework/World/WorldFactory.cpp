// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/World/WorldFactory.h"
#include "Framework/World/World.h"
#include "Utility/Log.h"

namespace we
{
	unique<World> WorldFactory::Create(const string& Name, EngineSubsystem& Sub)
	{
		auto it = Factories.find(Name);
		if (it == Factories.end())
		{
			ERROR("World '{}' not registered in factory", Name);
			return nullptr;
		}
		return it->second(Sub);
	}

	bool WorldFactory::IsRegistered(const string& Name) const
	{
		return Factories.contains(Name);
	}
}
