// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/CameraSubsystem.h"

namespace we
{
    CameraSubsystem::CameraSubsystem()
        : RenderResolution{ 1920,1080 }
    {
        SetCameraView(vec2u(RenderResolution));
    }

    void CameraSubsystem::SetCameraView(vec2u WindowSize)
    {
        CameraView.setSize({ (RenderResolution.x), (RenderResolution.y) });
        CameraView.setCenter({ (RenderResolution.x) * 0.5f, (RenderResolution.y) * 0.5f });
        float WindowRatio = (float)WindowSize.x / (float)WindowSize.y;
        float ViewRatio = RenderResolution.x / RenderResolution.y;

        float SizeX = 1.f;
        float SizeY = 1.f;
        float PosX = 0.f;
        float PosY = 0.f;

        if (WindowRatio > ViewRatio)
        {
            SizeX = ViewRatio / WindowRatio;
            PosX = (1.f - SizeX) * 0.5f;
        }
        else
        {
            SizeY = WindowRatio / ViewRatio;
            PosY = (1.f - SizeY) * 0.5f;
        }

        CameraView.setViewport({ {PosX, PosY}, {SizeX, SizeY} });
        OnViewUpdate.Broadcast(CameraView);
    }

    const view& CameraSubsystem::GetView() const
    {
        return CameraView;
    }
}