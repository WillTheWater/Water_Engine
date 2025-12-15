#pragma once
#include "Framework/Core.h"
#include <SFML/Graphics.hpp> 

namespace we
{
	class Renderer
	{
	public:
		explicit Renderer(sf::RenderWindow& Window);
		~Renderer() = default;

		void Clear();
		void Display();

		void DrawSprite(const sf::Sprite& SpriteToDraw);
		void DrawDebugShape(const sf::VertexArray& Vertices);

	private:
		sf::RenderWindow& Window;
	};
}