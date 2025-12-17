#pragma once
#include "Framework/Object.h"
#include "Framework/Delegate.h"

namespace we
{
	class World;

	class Level : public Object
	{
	public:
		Level(World* World);

		const World* GetWorld() const { return LevelWorld; }
		World* GetWorld() { return LevelWorld; }

		Delegate<> OnLevelEnd;

		virtual void BeginLevel();
		virtual void TickLevel(float DeltaTime);

		void LevelEnd();
		bool IsLevelFinished() const { return bLevelFinished; }

	private:
		World* LevelWorld;
		bool bLevelFinished;
		virtual void EndLevel();
	};
}