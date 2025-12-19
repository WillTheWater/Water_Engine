#pragma once
#include "GameMode/Level.h"

namespace we
{
	class LevelTransition : public Level
	{
	public:
		LevelTransition(World* World, float TransitionTime = 5.f);

		virtual void BeginLevel() override;
	private:
		float TransitionTime;
	};
}