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
		//LOG("Level Begin")
	}

	void Level::TickLevel(float DeltaTime)
	{
		//LOG("Level Tick")
	}

	void Level::LevelEnd()
	{
		OnLevelEnd.Broadcast();
		bLevelFinished = true;
		EndLevel();
	}

	void Level::EndLevel()
	{
		//LOG("Level Has Ended")
	}
}