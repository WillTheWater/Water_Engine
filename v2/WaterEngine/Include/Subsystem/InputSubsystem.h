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
    class InputSubsystem;

    // RAII handle for input bindings - auto-unbinds on destruction
    class BindingHandle
    {
    public:
        BindingHandle() = default;
        ~BindingHandle() { Release(); }

        // Move-only
        BindingHandle(BindingHandle&& Other) noexcept;
        BindingHandle& operator=(BindingHandle&& Other) noexcept;

        BindingHandle(const BindingHandle&) = delete;
        BindingHandle& operator=(const BindingHandle&) = delete;

        void Release();  // Manual unbind
        bool IsValid() const { return Owner != nullptr; }

    private:
        friend class InputSubsystem;
        BindingHandle(int ActionId, uint64_t Token, InputSubsystem* Owner);

        int ActionId = -1;
        uint64_t Token = 0;
        InputSubsystem* Owner = nullptr;
    };

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
        
        // Clear all held inputs (call when window loses focus)
        void ClearAllInput();

        // Event-driven binding with RAII handle - using member function
        template<typename ClassType>
        BindingHandle BindAction(int InputAction, ClassType* Object, void(ClassType::* Method)())
        {
            return BindAction(InputAction, [Object, Method]() {
                (Object->*Method)();
            });
        }

    private:
        friend class BindingHandle;
        void UnbindAction(int ActionId, uint64_t Token);

        // Non-template implementation
        BindingHandle BindAction(int InputAction, std::function<void()> Callback);

        static InputSubsystem* Instance;
        library<int, Input::Binding> Bindings;
        set<int> HeldActions;
        dictionary<int, uint> ActionPressCount;

        // Token-based callback storage for event-driven input
        dictionary<int, dictionary<uint64_t, std::function<void()>>> ActionCallbacks;
        uint64_t NextToken = 1;
    };

    inline InputSubsystem& InputController() { return InputSubsystem::Get(); }
}
