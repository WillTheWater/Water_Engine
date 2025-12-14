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
		Actor(World* OwningWorld, const string& TexturePath = "");
		virtual ~Actor();

		void BeginPlayGlobal();
		void TickGlobal(float DeltaTime);

		virtual void BeginPlay();
		virtual void Tick(float DeltaTime);

		bool IsPendingDestroy() const { return bPendingDestroy; }
		void Destroy() { bPendingDestroy = true; }

		World* GetWorld() const { return OwningWorld; }
		sf::Vector2u GetWindowSize() const;

		void SetTexture(const std::string& TexturePath, float SpriteScale = 1.0f);
		void SetSpriteFrame(int FrameWidth, int FrameHeight);
		void SetActorScale(float NewScale);
		void Render(class Renderer& GameRenderer);
		sf::FloatRect GetSpriteBounds() const;

		void SetActorLocation(const sf::Vector2f& NewLocation);
		void SetActorRotation(const sf::Angle& NewRotation);
		sf::Vector2f GetActorLocation() const;
		sf::Angle GetActorRotation() const;
		void AddActorLocationOffset(const sf::Vector2f& Offset);
		void AddActorRotationOffset(const sf::Angle& RotOffset);
		sf::Vector2f GetActorFowardVector() const;
		sf::Vector2f GetActorRightVector() const;

		void SetPhysicsTransform(const b2Vec2& Position, const b2Rot& Rotation);

		void SetEnablePhysics(bool Enabled);
		b2BodyId GetPhysicsBodyId() const { return APhysicsBody; }


	protected:
		void CenterPivot();
		bool IsOutOfBounds() const;

		void InitializePhysics();
		void UninitializePhysics();
		void UpdatePhysicsTransforms();

	private:
		World* OwningWorld;
		bool bHasBegunPlay;
		bool bPendingDestroy = false;

		shared<sf::Texture> ATexture;
		shared<sf::Sprite> ASprite;
		sf::Vector2i FrameSize;

		bool bPhysicsEnabled;
		b2BodyId APhysicsBody;
	};
}