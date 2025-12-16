#pragma once
#include "Framework/Core.h"
#include <SFML/Graphics.hpp>
#include "Framework/Object.h"
#include "EActorTypes.h"

class b2Body;

namespace we
{
	class World;

	class Actor : public Object
	{
	public:
		Actor(World* OwningWorld, const string& TexturePath = "");
		virtual ~Actor();

		void BeginPlayGlobal();
		void TickGlobal(float DeltaTime);

		virtual void BeginPlay();
		virtual void Tick(float DeltaTime);
		virtual void Destroy() override;

		World* GetWorld() const { return OwningWorld; }
		sf::Vector2u GetWindowSize() const;

		void SetTexture(const std::string& TexturePath, float SpriteScale = 1.0f);
		void SetSpriteFrame(int FrameWidth, int FrameHeight);
		void SetActorScale(float NewScale);
		void Render(class Renderer& GameRenderer);
		sf::FloatRect GetSpriteBounds() const;
		sf::Vector2u GetTextureSize() const { return ATexture->getSize(); }
		sf::Vector2f GetSpriteScale() const { return ASprite->getScale(); }

		EActorID GetActorID() const { return ActorID; }
		void SetActorID(EActorID NewID) { ActorID = NewID; }
		static EActorID GetNeutralActorID() { return NeutralID; }
		bool IsHostile(Actor* OtherActor);
		virtual void Damage(float Amount);

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

		void SetPhysicsEnabled(bool Enabled);
		virtual void OnActorBeginOverlap(Actor* OtherActor);
		virtual void OnActorEndOverlap(Actor* OtherActor);

	protected:
		void CenterPivot();
		bool IsOutOfBounds() const;

	private:
		World* OwningWorld;
		bool bHasBegunPlay;
		bool bDrawDebug = false;

		shared<sf::Texture> ATexture;
		shared<sf::Sprite> ASprite;
		sf::Vector2i FrameSize;
		sf::Vector2f LocalForward = { 1.f, 0.f };

		b2Body* PhysicsBody;
		void InitialziePhysics();
		void UninitialziePhysics();
		void UpdatePhysicsBodyTransform();
		bool bPhysicsEnabled;

		EActorID ActorID;
		const static EActorID NeutralID = EActorID::Neutral;
	};
}