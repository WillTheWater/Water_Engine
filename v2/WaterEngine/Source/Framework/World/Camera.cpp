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

		CamComp->BeginPlay();
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