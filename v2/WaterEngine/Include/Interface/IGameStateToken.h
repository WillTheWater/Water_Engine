// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
    class IGameStateToken
    {
    public:
        virtual ~IGameStateToken() = default;

        virtual size_t GetStateID() const = 0;
        virtual string GetDebugName() const = 0;

        template<typename T>
        bool IsA() const
        {
            return dynamic_cast<const T*>(this) != nullptr;
        }

        template<typename T>
        const T* As() const
        {
            return dynamic_cast<const T*>(this);
        }
    };
}