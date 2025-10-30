#pragma once

namespace we
{
	class World;

	class Actor
	{
	public:
		Actor(World* OwningWorld);
		virtual ~Actor();

		void BeginPlayGlobal();
		virtual void BeginPlay();
		virtual void Tick(float DeltaTime);

	private:
		World* OwningWorld;
		bool bHasBegunPlay;
	};
}