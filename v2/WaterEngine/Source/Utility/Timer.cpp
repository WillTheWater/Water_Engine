// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Utility/Timer.h"

namespace we
{
    uint TimerHandle::KeyIndex = 1;  // Start at 1 so default-constructed handle (0) is invalid
    TimerHandle::TimerHandle() : Key{ GetNextKey() } {}

    // =============================================================================
    // Timer Implementation
    // =============================================================================
    void TimerManager::Timer::Tick(float DeltaTime)
    {
        if (bExpired) return;
        
        Elapsed += DeltaTime;
        if (Elapsed >= Duration)
        {
            if (Callback) Callback();
            
            if (bLoop)
            {
                Elapsed = 0.0f;  // Restart
            }
            else
            {
                MarkExpired();    // One-shot done
            }
        }
    }

    // =============================================================================
    // Timer Manager Implementation
    // =============================================================================
    TimerManager& TimerManager::Get()
    {
        static TimerManager Instance;
        return Instance;
    }

    TimerHandle TimerManager::SetTimer(std::function<void()> Callback, float Duration, bool bLoop)
    {
        if (Duration <= 0.0f || !Callback) return TimerHandle();
        
        TimerHandle Handle;
        Timers.insert({ Handle, Timer(std::move(Callback), Duration, bLoop) });
        return Handle;
    }

    bool TimerManager::IsTimerActive(const TimerHandle& Handle) const
    {
        if (!Handle.IsValid()) return false;
        auto It = Timers.find(Handle);
        return It != Timers.end() && !It->second.IsExpired();
    }

    void TimerManager::ClearTimer(TimerHandle& Handle)
    {
        if (!Handle.IsValid()) return;
        
        auto Found = Timers.find(Handle);
        if (Found != Timers.end())
        {
            Found->second.MarkExpired();
        }
        Handle.Invalidate();
    }

    void TimerManager::ClearAllTimers()
    {
        Timers.clear();
    }

    void TimerManager::Tick(float DeltaTime)
    {
        for (auto It = Timers.begin(); It != Timers.end();)
        {
            It->second.Tick(DeltaTime);
            
            if (It->second.IsExpired())
            {
                It = Timers.erase(It);
            }
            else
            {
                ++It;
            }
        }
    }

    uint TimerManager::GetActiveTimerCount() const
    {
        return static_cast<uint>(Timers.size());
    }
}
