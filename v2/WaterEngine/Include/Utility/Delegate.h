// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
    template<typename... Args>
    class Delegate
    {
        struct IBinding
        {
            virtual ~IBinding() = default;
            virtual void Invoke(Args... args) = 0;
        };

        template<typename T>
        struct Binding : IBinding
        {
            T* Object;
            void (T::* Method)(Args...);

            Binding(T* Obj, void(T::* M)(Args...))
                : Object(Obj), Method(M) {
            }

            void Invoke(Args... args) override
            {
                (Object->*Method)(args...);
            }
        };

    public:
        template<typename T>
        void Bind(T* Obj, void(T::* Method)(Args...))
        {
            Bindings.push_back(we::make_unique<Binding<T>>(Obj, Method));
        }

        void Broadcast(Args... args)
        {
            for (auto& B : Bindings)
                B->Invoke(args...);
        }

    private:
        vector<unique<IBinding>> Bindings;
    };
}