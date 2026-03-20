// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/World/Camera.h"
#include "Component/CameraComponent.h"
#include "Utility/Log.h"

namespace we
{
	Camera::Camera(World& InWorld)
		: Actor(InWorld)
		, CamComp(make_shared<CameraComponent>(this))
	{
	}

	Camera::~Camera() = default;

	void Camera::BeginPlay()
	{
		Actor::BeginPlay();

		LOG("[Camera] BeginPlay on Actor {}", GetID());

		// Initialize the camera component
		CamComp->BeginPlay();
		
		// Auto-activate this camera (optional - can be removed if manual activation preferred)
		CamComp->SetActive();
	}

	void Camera::Tick(float DeltaTime)
	{
		Actor::Tick(DeltaTime);

		if (CamComp)
		{
			CamComp->Tick(DeltaTime);
		}
	}

	void Camera::EndPlay()
	{
		LOG("[Camera] EndPlay on Actor {}", GetID());

		if (CamComp)
		{
			CamComp->EndPlay();
			CamComp.reset();
		}

		Actor::EndPlay();
	}

	CameraComponent* Camera::GetCameraComponent() const
	{
		return CamComp.get();
	}

	void Camera::SetActive()
	{
		if (CamComp)
		{
			CamComp->SetActive();
		}
	}

	bool Camera::IsActive() const
	{
		if (CamComp)
		{
			return CamComp->IsActive();
		}
		return false;
	}
}
