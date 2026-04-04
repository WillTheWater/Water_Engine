// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Subsystem/InputSubsystem.h"

namespace we
{
	class UIController
	{
	public:
		UIController();
		~UIController();

		void Enable();
		void Disable();
		void Tick(float DeltaTime);

		bool IsEnabled() const { return bEnabled; }

	private:
		void OnSouthPressed();
		void PollStick(float DeltaTime);
		vec2f WindowToWorld(vec2f WindowPos);

		InputSubsystem& Input;
		BindingHandle SouthPressBinding;
		bool bEnabled = false;
		
		vec2f WindowCursorPos{960, 540};
		
		static constexpr float Speed = 800.0f;
		static constexpr float Deadzone = 15.0f;
	};
}
