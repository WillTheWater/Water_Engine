#include "GameMode/Level.h"
#include "Framework/Core.h"

namespace we
{
	Level::Level(World* World)
		: LevelWorld{World}
		, bLevelFinished{false}
	{
	}

	void Level::BeginLevel()
	{
	}
	void Level::TickLevel(float DeltaTime)
	{
	}

	void Level::LevelEnd()
	{
		bLevelFinished = true;
		EndLevel();
		OnLevelEnd.Broadcast();
	}

	void Level::EndLevel()
	{
	}
}