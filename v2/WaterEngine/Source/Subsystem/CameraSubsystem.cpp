// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/CameraSubsystem.h"

namespace we
{
    void CameraSubsystem::SetActiveCamera(ICamera* Camera)
    {
        ActiveCamera = Camera;
    }

    ICamera* CameraSubsystem::GetActiveCamera() const
    {
        return ActiveCamera;
    }

    bool CameraSubsystem::HasActiveCamera() const
    {
        return ActiveCamera != nullptr;
    }

    bool CameraSubsystem::GetCurrentView(CameraView& OutView) const
    {
        if (!ActiveCamera) 
        {
            return false;
        }
        
        OutView = ActiveCamera->CalculateView();
        return true;
    }

} // namespace we
