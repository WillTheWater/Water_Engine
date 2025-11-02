#pragma once
#include <SFML/Graphics.hpp>
#include "Framework/Core.h"

namespace we
{
	class AssetManager
	{
	public:
		static AssetManager& GetAssetManager();
		shared<sf::Texture> LoadTexture(const std::string& TexturePath);

	protected:
		AssetManager();

	private:
		static unique<AssetManager> UAssetManager;
		Dictionary<std::string, shared<sf::Texture>> LoadedTextures;
	};
}