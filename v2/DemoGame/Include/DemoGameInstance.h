// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/GameInstance.h"

namespace we
{
	class DemoGameInstance : public GameInstance
	{
	public:
		DemoGameInstance();

		void Init(EngineSubsystem& InSubsystem) override;
		void Shutdown() override;
	};
}