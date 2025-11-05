#pragma once
#include "Framework/Object.h"
#include "SFML/Graphics.hpp"
#include "Framework/Core.h"

namespace we
{
	class World;

	class Actor : public Object
	{
	public:
		Actor(World* OwningWorld, const std::string& TexturePath = "");
		virtual ~Actor();

		void BeginPlayGlobal();
		void TickGlobal(float DeltaTime);
		virtual void BeginPlay();
		virtual void Tick(float DeltaTime);

		void SetTexture(const std::string& TexturePath, int FrameWidth = 0, int FrameHeight = 0, float SpriteScale = 1.f);
		void SetActorScale(float NewScale);
		void Render(sf::RenderWindow& Window);

		void SetActorLocation(const sf::Vector2f& NewLocation);
		void SetActorRotation(const sf::Angle& NewRotation);
		sf::Vector2f GetActorLocation() const;
		sf::Angle GetActorRotation() const;
		void AddActorLocationOffset(const sf::Vector2f& Offset);
		void AddActorRotationOffset(const sf::Angle& RotOffset);
		sf::Vector2f GetActorFowardVector() const;
		sf::Vector2f GetActorRightVector() const;

	private:
		World* OwningWorld;
		bool bHasBegunPlay;

		shared<sf::Texture> ATexture;
		shared<sf::Sprite> ASprite;
		void CenterPivot();

		sf::Vector2i FrameSize{ 0, 0 };
		int CurrentFrame = 0;
	};
}