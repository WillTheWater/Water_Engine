#pragma once

namespace we
{
	class Application;

	class World
	{
	public:
		World(Application* OwningApp);
		virtual ~World();

		void BeginPlayGlobal();
		void TickGlobal(float DeltaTime);

	private:
		void BeginPlay();
		void Tick(float DeltaTime);

		Application* OwningApp;
		bool bHasBegunPlay;
	};
}