// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/CameraSubsystem.h"

namespace we
{
	void CameraSubsystem::Update(vec2u WindowSize)
	{
        float WindowRatio = (float)WindowSize.x / (float)WindowSize.y;
        float ViewRatio = (float)RenderResolution.x / (float)RenderResolution.y;

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
	}

	const view& CameraSubsystem::GetView() const
	{
		return CameraView;
	}
}