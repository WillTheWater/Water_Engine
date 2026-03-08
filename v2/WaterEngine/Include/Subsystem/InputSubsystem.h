// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Input/InputBinding.h"
#include <SFML/Window/Event.hpp>

namespace we
{
	class InputSubsystem
	{
	public:
		InputSubsystem();

		void HandleEvent(const sf::Event& Event);

		static InputSubsystem& Get();

		void Bind(int InputAction, const Input::Binding& Binding);
		
		bool Pressed(int InputAction) const;
		void OnPressed(const Input::Binding& Binding);
		void OnReleased(const Input::Binding& Binding);

	private:
		static InputSubsystem* Instance;
		library<int, Input::Binding> Bindings;
		set<int> HeldActions;
		dictionary<int, uint> ActionPressCount;
	};

	inline InputSubsystem& InputController() { return InputSubsystem::Get(); }
}
