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
		return LoadAsset<sf::Texture>(TexturePath, LoadedTextures,
			[](sf::Texture& tex, const string& path)
			{
				return tex.loadFromFile(path);
			}
		);
	}

	shared<sf::Font> AssetManager::LoadFont(const string& FontPath)
	{
		return LoadAsset<sf::Font>(FontPath, LoadedFonts,
			[](sf::Font& font, const string& path)
			{
				return font.openFromFile(path);
			}
		);
	}

	void AssetManager::GarbageCollectionCycle()
	{
		GarbageCollect(LoadedTextures);
		GarbageCollect(LoadedFonts);
	}

	void AssetManager::SetAssetRootDirctory(const std::string& Directory)
	{
		RootDirectory = Directory;
	}

	AssetManager::AssetManager()
		: RootDirectory{}
	{

	}
}