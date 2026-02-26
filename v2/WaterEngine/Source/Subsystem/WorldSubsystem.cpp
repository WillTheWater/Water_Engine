// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/WorldSubsystem.h"
#include "Framework/World/World.h"
#include "Framework/EngineSubsystem.h"
#include "Utility/Log.h"

namespace we
{
	WorldSubsystem::WorldSubsystem(EngineSubsystem& InSubsystem)
		: Subsystem{ InSubsystem }
	{
	}

	void WorldSubsystem::RegisterWorld(size_t StateID, WorldFactory Factory)
	{
		WorldRegistry[StateID] = std::move(Factory);
	}

	void WorldSubsystem::LoadWorldForState(const IGameStateToken* State)
	{
		// TODO: Implement when IGameStateToken is defined
		LOG("LoadWorldForState: Not yet implemented");
	}

	void WorldSubsystem::UnloadWorld()
	{
		CurrentWorld = nullptr;
	}
}
