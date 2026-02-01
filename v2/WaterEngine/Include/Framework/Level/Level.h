// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"
#include "Framework/EngineSubsystem.h"

namespace we
{
	class Level
	{
	public:
		Level(EngineSubsystem& Subsystem) : Subsystem{Subsystem}{}
		virtual ~Level() = default;

		virtual void BeginPlay() = 0;
		virtual void Tick(float DeltaTime) = 0;
		virtual void Render() = 0;
		virtual void HandleEvent(const sf::Event&) {};
		virtual void OnPause(bool) {}
		virtual void Cleanup() {};

	protected:
		EngineSubsystem& Subsystem;
	};
}