// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"
#include <Framework/WaterEngine.h>

namespace we
{
	class Game : public WaterEngine
	{
	public:
		Game();

	private:
		void OnStateEnter();
	};
}