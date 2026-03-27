// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/InputSubsystem.h"
#include "EventHandler/InputEventHandler.h"

namespace we
{
    BindingHandle::BindingHandle(int InActionId, uint64_t InToken, InputSubsystem* InOwner)
        : ActionId(InActionId)
        , Token(InToken)
        , Owner(InOwner)
    {
    }

    BindingHandle::BindingHandle(BindingHandle&& Other) noexcept
        : ActionId(Other.ActionId)
        , Token(Other.Token)
        , Owner(Other.Owner)
    {
        Other.ActionId = -1;
        Other.Token = 0;
        Other.Owner = nullptr;
    }

    BindingHandle& BindingHandle::operator=(BindingHandle&& Other) noexcept
    {
        if (this != &Other)
        {
            Release();
            ActionId = Other.ActionId;
            Token = Other.Token;
            Owner = Other.Owner;
            Other.ActionId = -1;
            Other.Token = 0;
            Other.Owner = nullptr;
        }
        return *this;
    }

    void BindingHandle::Release()
    {
        if (Owner && ActionId >= 0 && Token != 0)
        {
            Owner->UnbindAction(ActionId, Token);
            Owner = nullptr;
            ActionId = -1;
            Token = 0;
        }
    }

    InputSubsystem* InputSubsystem::Instance = nullptr;

    InputSubsystem::InputSubsystem()
    {
        Instance = this;
    }

    InputSubsystem& InputSubsystem::Get()
    {
        return *Instance;
    }

    void InputSubsystem::HandleEvent(const sf::Event& event)
    {
        InputEventHandler Handler{ *this };
        event.visit(Handler);
    }

    void InputSubsystem::Bind(int InputAction, const Input::Binding& Binding)
    {
        Bindings.emplace(InputAction, Binding);
    }

    bool InputSubsystem::Pressed(int InputAction) const
    {
        return HeldActions.contains(InputAction);
    }

    void InputSubsystem::OnPressed(const Input::Binding& Binding)
    {
        for (const auto& [Action, BoundBinding] : Bindings)
        {
            if (Input::BindingsEqual(BoundBinding, Binding))
            {
                uint& Count = ActionPressCount[Action];
                if (Count == 0)
                {
                    HeldActions.insert(Action);

                    auto It = ActionCallbacks.find(Action);
                    if (It != ActionCallbacks.end())
                    {
                        for (auto& [Token, Callback] : It->second)
                        {
                            Callback();
                        }
                    }
                }
                Count++;
            }
        }
    }

    void InputSubsystem::OnReleased(const Input::Binding& Binding)
    {
        for (const auto& [Action, BoundBinding] : Bindings)
        {
            if (Input::BindingsEqual(BoundBinding, Binding))
            {
                auto It = ActionPressCount.find(Action);
                if (It != ActionPressCount.end())
                {
                    if (It->second > 0)
                    {
                        It->second--;
                    }
                    if (It->second == 0)
                    {
                        HeldActions.erase(Action);
                    }
                }
            }
        }
    }

    BindingHandle InputSubsystem::BindAction(int InputAction, std::function<void()> Callback)
    {
        uint64_t Token = NextToken++;
        ActionCallbacks[InputAction][Token] = std::move(Callback);
        return BindingHandle(InputAction, Token, this);
    }

    void InputSubsystem::UnbindAction(int ActionId, uint64_t Token)
    {
        auto It = ActionCallbacks.find(ActionId);
        if (It != ActionCallbacks.end())
        {
            It->second.erase(Token);
            if (It->second.empty())
            {
                ActionCallbacks.erase(It);
            }
        }
    }
}