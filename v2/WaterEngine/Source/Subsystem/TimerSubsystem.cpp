// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/TimerSubsystem.h"

namespace we
{
    TimerSubsystem::TimerSubsystem()
        : Timers{}
    {
    }

    unsigned int TimerHandle::TimerKeyIndex = 0;

    TimerHandle::TimerHandle()
        : TimerKey{ GetNextTimerKey() }
    {
    }

    Timer::Timer(weak<Object> ObjectRef, std::function<void()> Callback, float Duration, bool Loop)
        : Listener{ ObjectRef, Callback }
        , Duration{ Duration }
        , bLoop{ Loop }
        , TimerCounter{ 0.f }
        , bIsExpired{ false }
    {
    }

    void Timer::TickTimer(float DeltaTime)
    {
        if (Expired()) { return; }
        TimerCounter += DeltaTime;
        if (TimerCounter >= Duration)
        {
            Listener.second();
            if (bLoop)
            {
                TimerCounter = 0.f;
            }
            else
            {
                SetExpired();
            }
        }
    }

    bool Timer::Expired() const
    {
        return bIsExpired || Listener.first.expired() || Listener.first.lock()->IsPendingDestroy();
    }

    void Timer::SetExpired()
    {
        bIsExpired = true;
    }

    void TimerSubsystem::Update(float DeltaTime)
    {
        for (auto Timer = Timers.begin(); Timer != Timers.end();)
        {
            if (Timer->second.Expired())
            {
                Timer = Timers.erase(Timer);
            }
            else
            {
                Timer->second.TickTimer(DeltaTime);
                ++Timer;
            }
        }
        
        GCTimer += DeltaTime;
        if (GCTimer >= GCInterval)
        {
            GCTimer = 0.0f;
            TriggerGarbageCollection.Broadcast();
        }
    }

    void TimerSubsystem::ClearTimer(TimerHandle Handle)
    {
        auto TimerHash = Timers.find(Handle);
        if (TimerHash != Timers.end())
        {
            TimerHash->second.SetExpired();
        }
    }

    TimerSubsystem& TimerSubsystem::Get()
    {
        static TimerSubsystem Instance;
        return Instance;
    }

    bool operator==(const TimerHandle& lhs, const TimerHandle& rhs)
    {
        return lhs.GetTimerKey() == rhs.GetTimerKey();
    }
}