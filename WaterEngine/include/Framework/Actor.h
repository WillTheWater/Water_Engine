// Copyright (C) 2025 Stoic Ronin Studio. All Rights Reserved.

#pragma once

namespace we
{
	class World;

	class Actor
	{
	public:
		Actor(World* OwningWorld);

		void BeginPlayGlobal();
		virtual void BeginPlay();
		virtual void Tick(float DeltaTime);

	private:
		World* OwningWorld;
		bool bHasBegunPlay;
	};
}