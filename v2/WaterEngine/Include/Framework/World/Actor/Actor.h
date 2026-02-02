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

	public:
		World* GetWorld() const { return OwnerWorld; }
		const sprite& GetSprite() const { return Sprite; }

		void SetTexture();
		void SetPosition(vec2f Pos);

	private:
		World* OwnerWorld;
		shared<texture> Texture;
		sprite Sprite;
		bool bHasBegunPlay;
		vec2f Position;
	};
}