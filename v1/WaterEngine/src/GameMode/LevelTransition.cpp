#include "GameMode/LevelTransition.h"
#include "Framework/TimerManager.h"

namespace we
{
	LevelTransition::LevelTransition(World* World, float TransitionTime)
		: Level{World}
		, TransitionTime{TransitionTime}
	{
	}

	void LevelTransition::BeginLevel()
	{
		TimerManager::Get().SetTimer(GetObject(), &LevelTransition::LevelEnd, TransitionTime);
	}
}