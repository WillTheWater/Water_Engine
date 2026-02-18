// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
    class Object;

    // =============================================================================
    // Timer Handle - Lightweight identifier for active timers
    // =============================================================================
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

    // =============================================================================
    // Timer Manager - Global timer system
    // 
    // Usage:
    //   // One-shot timer
    //   SetTimer([](){ LOG("Hello!"); }, 2.0f);
    //
    //   // Looping timer
    //   SetTimer([](){ LOG("Tick"); }, 1.0f, true);
    //
    //   // Member function timer (auto-cancels if object destroyed)
    //   SetTimer(PlayerPtr, &Player::RegenHealth, 5.0f, true);
    //
    //   // Store handle to cancel later
    //   TimerHandle Handle = SetTimer(...);
    //   ClearTimer(Handle);
    // =============================================================================
    class TimerManager
    {
    public:
        static TimerManager& Get();

        // Set a timer with a lambda/free function
        // Returns handle for cancellation
        TimerHandle SetTimer(std::function<void()> Callback, float Duration, bool bLoop = false);

        // Set a timer bound to an object (auto-cancels when object destroyed)
        template<typename ClassType>
        TimerHandle SetTimer(shared<ClassType> Target, void(ClassType::* Method)(), float Duration, bool bLoop = false)
        {
            if (!Target) return TimerHandle();
            
            weak<ClassType> WeakTarget = Target;
            return SetTimer([WeakTarget, Method]()
            {
                if (auto Shared = WeakTarget.lock())
                {
                    (Shared.get()->*Method)();
                }
            }, Duration, bLoop);
        }

        // Check if a timer is still active
        bool IsTimerActive(const TimerHandle& Handle) const;

        // Cancel a specific timer
        void ClearTimer(TimerHandle& Handle);

        // Cancel all timers
        void ClearAllTimers();

        // Called once per frame by engine
        void Tick(float DeltaTime);

        // Get count of active timers
        uint GetActiveTimerCount() const;

    private:
        struct Timer
        {
            std::function<void()> Callback;
            float Duration;
            float Elapsed = 0.0f;
            bool bLoop;
            bool bExpired = false;

            Timer(std::function<void()> InCallback, float InDuration, bool bInLoop)
                : Callback(std::move(InCallback)), Duration(InDuration), bLoop(bInLoop) {}

            void Tick(float DeltaTime);
            bool IsExpired() const { return bExpired; }
            void MarkExpired() { bExpired = true; }
        };

        TimerManager() = default;
        dictionary<TimerHandle, Timer, TimerHandleHash> Timers;
    };

    // Global shortcut: SetTimer(...)
    inline TimerHandle SetTimer(std::function<void()> Callback, float Duration, bool bLoop = false)
    {
        return TimerManager::Get().SetTimer(std::move(Callback), Duration, bLoop);
    }

    template<typename ClassType>
    inline TimerHandle SetTimer(shared<ClassType> Target, void(ClassType::* Method)(), float Duration, bool bLoop = false)
    {
        return TimerManager::Get().SetTimer(Target, Method, Duration, bLoop);
    }
}