// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"

namespace we
{
    class Object;

    class TimerHandle
    {
    public:
        TimerHandle();

        uint GetKey() const { return Key; }
        bool operator==(const TimerHandle& Other) const { return Key == Other.Key; }
        bool IsValid() const { return Key != 0; }
        void Invalidate() { Key = 0; }

    private:
        uint Key;
        static uint KeyIndex;
        static uint GetNextKey() { return KeyIndex++; }
    };

    struct TimerHandleHash
    {
        size_t operator()(const TimerHandle& Handle) const
        {
            return std::hash<uint>{}(Handle.GetKey());
        }
    };

    struct Timer
    {
        weak<Object> Target;
        std::function<void()> Callback;
        float Duration;
        float Elapsed;
        bool bLoop;
        bool bExpired;

        Timer(weak<Object> InTarget, std::function<void()> InCallback, float InDuration, bool bInLoop);
        void Tick(float DeltaTime);
        bool IsExpired() const;
        void MarkExpired();
    };

    class TimerManager
    {
    public:
        static TimerManager& Get();

        template<typename ClassType>
        TimerHandle SetTimer(weak<Object> Target, void(ClassType::* Method)(), float Duration, bool bLoop = false)
        {
            TimerHandle Handle;
            Timers.insert({ Handle, Timer(Target, [Target, Method]()
            {
                if (auto Shared = Target.lock())
                {
                    (static_cast<ClassType*>(Shared.get())->*Method)();
                }
            }, Duration, bLoop) });
            return Handle;
        }

        TimerHandle SetTimer(std::function<void()> Callback, float Duration, bool bLoop = false);
        void Tick(float DeltaTime);
        void ClearTimer(TimerHandle& Handle);
        void ClearAllTimers();

    private:
        TimerManager() = default;
        dictionary<TimerHandle, Timer, TimerHandleHash> Timers;
    };
}