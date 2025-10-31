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

		void SetTexture(const std::string& TexturePath);
		void Render(sf::RenderWindow& Window);

	private:
		World* OwningWorld;
		bool bHasBegunPlay;

		sf::Texture ATexture;
		sf::Sprite ASprite;
	};
}