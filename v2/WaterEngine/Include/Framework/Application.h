// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"

#include "Framework/GameWindow.h"

namespace we
{
	class Application
	{
	public:
		Application();
		~Application(){}

		void Run();

	private:
		unique<GameWindow> mGameWindow;
	};
}