// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Interface/Actor/IActorComponent.h"

namespace we
{
    class CameraSubsystem;

    class CameraComponent : public IActorComponent
    {
    public:
        explicit CameraComponent(Actor* InOwner);
        ~CameraComponent() override;

        // IActorComponent
        void BeginPlay() override;
        void Tick(float DeltaTime) override;
        void EndPlay() override;
        Actor* GetOwner() const override;

        // Activation
        void SetActive();
        bool IsActive() const;

        // Basic settings
        void SetOffset(vec2f InOffset);
        void SetZoom(float InZoom);
        void SetRotation(float InRadians);

        // Smooth follow
        void SetSmoothFollow(bool bEnable, float SmoothTime = 0.3f);

        // Shake effect
        void Shake(float Amplitude, float Duration, float Frequency = 20.0f);
        void StopShake();

        // Bounds clamping
        void SetBounds(rectf WorldBounds);
        void ClearBounds();

        // Queries (used by CameraSubsystem)
        vec2f GetFinalPosition() const;
        float GetZoom() const { return Zoom; }
        float GetRotation() const { return Rotation; }

    private:
        void UpdatePosition(float DeltaTime);
        vec2f ApplyShake() const;
        void ClampToBounds(vec2f& Position) const;

    private:
        Actor* Owner;
        CameraSubsystem* CameraSys = nullptr;

        // Config
        vec2f Offset = {0.0f, 0.0f};
        float Zoom = 1.0f;
        float Rotation = 0.0f;

        // Smooth follow
        bool bSmoothFollow = false;
        float SmoothTime = 0.3f;
        vec2f SmoothedPosition;

        struct ShakeState
        {
            float Amplitude;
            float Duration;
            float Frequency;
            float Elapsed;
        };
        optional<ShakeState> ActiveShake;

        optional<rectf> Bounds;
    };
}