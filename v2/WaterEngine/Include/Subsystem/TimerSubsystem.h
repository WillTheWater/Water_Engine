// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/Object.h"

namespace we
{
    struct TimerHandle
    {
    public:
        TimerHandle();
        unsigned int GetTimerKey() const { return TimerKey; }

    private:
        unsigned int TimerKey;
        static unsigned int TimerKeyIndex;
        static unsigned int GetNextTimerKey() { return TimerKeyIndex++; }
    };

    struct TimerHashFunction
    {
    public:
        std::size_t operator()(const TimerHandle& Handle) const
        {
            return Handle.GetTimerKey();
        }
    };

    bool operator==(const TimerHandle& lhs, const TimerHandle& rhs);

    struct Timer
    {
    public:
        Timer(weak<Object> ObjectRef, std::function<void()> Callback, float Duration, bool Loop);
        void TickTimer(float DeltaTime);
        bool Expired() const;
        void SetExpired();

    private:
        pair<weak<Object>, std::function<void()>> Listener;
        float Duration;
        bool bLoop;
        float TimerCounter;
        bool bIsExpired;
    };

    class TimerSubsystem
    {
    public:
        TimerSubsystem();
        static TimerSubsystem& Get();

        template<typename ClassName>
        TimerHandle SetTimer(weak<Object> ObjectRef, void(ClassName::* Callback)(), float Duration, bool Loop = false)
        {
            TimerHandle NewHandle{};
            Timers.insert({ NewHandle, Timer(ObjectRef, [=] {(static_cast<ClassName*>(ObjectRef.lock().get())->*Callback)(); }, Duration, Loop) });
            return NewHandle;
        }

        void Update(float DeltaTime);
        void ClearTimer(TimerHandle Handle);

    private:
        dictionary<TimerHandle, Timer, TimerHashFunction> Timers;
    };

    inline TimerSubsystem& GetTimer() { return TimerSubsystem::Get(); }
}
