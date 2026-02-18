// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/Object/Object.h"

namespace we
{
	class World;

	class Actor : public Object
	{
	public:
		Actor(World* OwningWorld, const string& TexturePath);
		virtual ~Actor() = default;

		// Lifecycle
		virtual void BeginPlay();
		virtual void Tick(float DeltaTime);

		// Transform
		void SetPosition(const vec2f& Position);
		vec2f GetPosition() const { return ActorPosition; }

		void SetRotation(angle Rotation);
		angle GetRotation() const { return ActorRotation; }

		void SetScale(const vec2f& Scale);
		vec2f GetScale() const { return ActorScale; }

		// Rendering
		virtual const sf::Drawable* GetDrawable() const;
		virtual float GetRenderDepth() const { return ActorPosition.y; }
		void SetVisible(bool bVisible) { bIsVisible = bVisible; }
		bool IsVisible() const { return bIsVisible; }

		// Animation support
		void SetSpriteTexture(shared<texture> NewTexture);
		void SetSpriteTextureRect(const recti& TexRect);
		void SetOrigin(const vec2f& Origin);

		World* GetWorld() const { return OwningWorld; }

	protected:
		void UpdateSpriteTransform();
		sprite& GetSprite() { return ActorSprite; }

	private:
		void Initialize();

	private:
		World* OwningWorld;
		shared<texture> ActorTexture;
		sprite ActorSprite;
		vec2f ActorPosition;
		angle ActorRotation;
		vec2f ActorScale;
		bool bIsVisible = true;
	};
}