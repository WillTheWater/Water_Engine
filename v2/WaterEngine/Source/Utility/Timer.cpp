// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Utility/Timer.h"
#include "Framework/World/Object/Object.h"

namespace we
{
    uint TimerHandle::KeyIndex = 0;
    TimerHandle::TimerHandle() : Key{ GetNextKey() } {}

    Timer::Timer(weak<Object> InTarget, std::function<void()> InCallback, float InDuration, bool bInLoop)
        : Target{ InTarget }, Callback{ std::move(InCallback) }, Duration{ InDuration }, Elapsed{ 0.f }, bLoop{ bInLoop }, bExpired{ false }
    {
    }

    void Timer::Tick(float DeltaTime)
    {
        if (IsExpired()) return;
        Elapsed += DeltaTime;
        if (Elapsed >= Duration)
        {
            Callback();
            if (bLoop) Elapsed = 0.f;
            else MarkExpired();
        }
    }

    bool Timer::IsExpired() const
    {
        return bExpired || Target.expired() || (Target.lock() && Target.lock()->IsPendingDestroy());
    }

    void Timer::MarkExpired() { bExpired = true; }

    TimerManager& TimerManager::Get()
    {
        static TimerManager Instance;
        return Instance;
    }

    TimerHandle TimerManager::SetTimer(std::function<void()> Callback, float Duration, bool bLoop)
    {
        TimerHandle Handle;
        Timers.insert({ Handle, Timer(weak<Object>{}, std::move(Callback), Duration, bLoop) });
        return Handle;
    }

    void TimerManager::Tick(float DeltaTime)
    {
        for (auto It = Timers.begin(); It != Timers.end();)
        {
            if (It->second.IsExpired()) It = Timers.erase(It);
            else { It->second.Tick(DeltaTime); ++It; }
        }
    }

    void TimerManager::ClearTimer(TimerHandle& Handle)
    {
        auto Found = Timers.find(Handle);
        if (Found != Timers.end()) Found->second.MarkExpired();
        Handle.Invalidate();
    }

    void TimerManager::ClearAllTimers() { Timers.clear(); }
}