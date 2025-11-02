#include "Framework/AssetManager.h"

namespace we
{
	unique<AssetManager> AssetManager::UAssetManager{nullptr};

	AssetManager& AssetManager::GetAssetManager()
	{
		if (!UAssetManager)
		{
			UAssetManager = std::move(unique<AssetManager>{new AssetManager});
		}
		return *UAssetManager;
	}

	shared<sf::Texture> AssetManager::LoadTexture(const std::string& TexturePath)
	{
		auto FoundTexture = LoadedTextures.find(TexturePath);
		if (FoundTexture != LoadedTextures.end())
		{
			return FoundTexture->second;
		}
		shared<sf::Texture> NewTexture{ new sf::Texture };
		if (NewTexture->loadFromFile(TexturePath))
		{
			LoadedTextures.insert({ TexturePath, NewTexture });
			return NewTexture;
		}
		return shared<sf::Texture> {nullptr};
	}

	AssetManager::AssetManager()
	{

	}
}