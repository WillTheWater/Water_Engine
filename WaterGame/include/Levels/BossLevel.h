#pragma once
#include "GameMode/Level.h"
#include "SFML/Graphics.hpp"
#include "Framework/TimerManager.h"

namespace we
{
	class Actor;

	class BossLevel : public Level
	{
	public:
		BossLevel(World* World);

		virtual void BeginLevel() override;

	private:
		void BossDefeated(Actor* BossActor);
	};
}