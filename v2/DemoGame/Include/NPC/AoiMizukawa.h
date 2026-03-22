// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/Character.h"

namespace we
{
	class AoiMizukawa : public Character
	{
	public:
		AoiMizukawa(World& OwningWorld);
		~AoiMizukawa();

	protected:
		void BeginPlay() override;
		void Tick(float DeltaTime) override;
		void EndPlay() override;

	private:
		void SetupAnimation();
	};
}