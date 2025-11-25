#pragma once
#include "Framework/Core.h"
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

namespace we
{
	class World;

	class Actor
	{
	public:
		Actor(World* OwningWorld, const std::string& TexturePath = "");
		virtual ~Actor();

		// Game Loop
		void BeginPlayGlobal();
		void TickGlobal(float DeltaTime);

		// Virtual Hooks
		virtual void BeginPlay();
		virtual void Tick(float DeltaTime);

		// Core State
		bool IsPendingDestroy() const { return bPendingDestroy; }
		void Destroy() { bPendingDestroy = true; }

		// World Access
		World* GetWorld() const { return OwningWorld; }
		sf::Vector2u GetWindowSize() const;

		// Rendering & Assets
		void SetTexture(const std::string& TexturePath, float SpriteScale = 1.0f);
		void SetSpriteFrame(int FrameWidth, int FrameHeight);
		void SetActorScale(float NewScale);
		void Render(class Renderer& GameRenderer);
		sf::FloatRect GetSpriteBounds() const;

		// Transform
		void SetActorLocation(const sf::Vector2f& NewLocation);
		void SetActorRotation(const sf::Angle& NewRotation);
		sf::Vector2f GetActorLocation() const;
		sf::Angle GetActorRotation() const;
		void AddActorLocationOffset(const sf::Vector2f& Offset);
		void AddActorRotationOffset(const sf::Angle& RotOffset);
		sf::Vector2f GetActorFowardVector() const;
		sf::Vector2f GetActorRightVector() const;

		// Physics Interface (Crucial for synchronization from PhysicsSystem)
		void SetPhysicsTransform(const b2Vec2& Position, const b2Rot& Rotation);

		// Physics Management (New)
		void SetEnablePhysics(bool Enabled);
		b2BodyId GetPhysicsBodyId() const { return APhysicsBody; }


	protected:
		// Utility
		void CenterPivot();
		bool IsOutOfBounds() const;

		// Physics Helpers (Internal)
		void InitializePhysics();
		void UninitializePhysics();
		void UpdatePhysicsTransforms(); // For manually setting Box2D position from SFML

	private:
		// Core State
		World* OwningWorld;
		bool bHasBegunPlay;
		bool bPendingDestroy = false;

		// Rendering
		shared<sf::Texture> ATexture;
		shared<sf::Sprite> ASprite;
		sf::Vector2i FrameSize;

		// Physics
		bool bPhysicsEnabled;
		b2BodyId APhysicsBody; // The handle to the Box2D body
	};
}