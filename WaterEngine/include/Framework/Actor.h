#pragma once
#include "Framework/Core.h"
#include <SFML/Graphics.hpp>

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
		sf::Vector2u GetTextureSize() const { return ATexture->getSize(); }
		sf::Vector2f GetSpriteScale() const { return ASprite->getScale(); }

		void SetActorLocation(const sf::Vector2f& NewLocation);
		void SetActorRotation(const sf::Angle& NewRotation);
		sf::Vector2f GetActorLocation() const;
		sf::Angle GetActorRotation() const;
		void AddActorLocationOffset(const sf::Vector2f& Offset);
		void AddActorRotationOffset(const sf::Angle& RotOffset);
		void SetLocalForwardVector(sf::Vector2f& Forward);
		sf::Vector2f GetActorFowardVector() const;
		sf::Vector2f GetActorRightVector() const;
		sf::VertexArray ForwardVectorDebugShape(float Length = 150.f, sf::Color Color = sf::Color::Red) const;
		void SetShouldDrawDebugShapes(bool ToDraw) { bDrawDebug = ToDraw; }

		void SetEnablePhysics(bool Enabled);

	protected:
		void CenterPivot();
		bool IsOutOfBounds() const;

	private:
		World* OwningWorld;
		bool bHasBegunPlay;
		bool bPendingDestroy = false;
		bool bDrawDebug = false;

		shared<sf::Texture> ATexture;
		shared<sf::Sprite> ASprite;
		sf::Vector2i FrameSize;
		sf::Vector2f LocalForward = { 1.f, 0.f };

		bool bPhysicsEnabled;
	};
}