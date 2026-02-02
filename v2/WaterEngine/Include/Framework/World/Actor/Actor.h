// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"
#include "Framework/World/Object/Object.h"

namespace we
{
	class World;

	class Actor : public Object
	{
	public:
		explicit Actor(World* OwningWorld, const string& TexturePath);
		virtual ~Actor() = default;

		virtual void BeginPlay();
		virtual void Tick(float DeltaTime);

		World* GetWorld() const { return OwnerWorld; }
		const sprite& GetSprite() const { return Sprite; }

	private:
		World* OwnerWorld;
		texture Texture;
		sprite Sprite;
		bool bHasBegunPlay;
	};
}