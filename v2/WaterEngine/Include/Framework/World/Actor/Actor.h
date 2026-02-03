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
		explicit Actor(World* OwningWorld);
		explicit Actor(World* OwningWorld, const string& TexturePath);
		virtual ~Actor();

		void BeginPlayGlobal();
		virtual void BeginPlay(){}
		virtual void Tick(float DeltaTime){}
		virtual void Destroy() override;

	public:
		// ================= TRANSFORMS =================
		void SetPosition(const vec2f& Position);
		vec2f GetPosition() const;

		void SetRotation(angle Angle);
		angle GetRotation() const;

		void SetScale(const vec2f& Scale);
		vec2f GetScale() const;

		rectf GetBounds() const;

		void SetOrigin(const vec2f& Origin);
		void CenterOrigin();

		void SetColor(const color& Color);

	public:
		World* GetWorld() const { return OwnerWorld; }
		bool HasSprite() const { return Sprite.has_value(); }
		const sprite& GetSprite() const { return *Sprite; }

		Delegate<Actor*> OnActorDestroyed;

	protected:
		World* OwnerWorld;

	private:
		shared<texture> Texture;
		optional<sprite> Sprite;
		bool bHasBegunPlay;
	};
}