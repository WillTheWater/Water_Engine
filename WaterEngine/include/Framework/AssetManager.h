#pragma once
#include <SFML/Graphics.hpp>
#include "Framework/Core.h"

namespace we
{
	class AssetManager
	{
	public:
		static AssetManager& Get();
		shared<sf::Texture> LoadTexture(const string& TexturePath);
		shared<sf::Font> LoadFont(const string& FontPath);
		void GarbageCollectionCycle();
		void SetAssetRootDirctory(const std::string& Directory);

	protected:
		AssetManager();

	private:
		template<typename Asset, typename MapType, typename LoadFunc>
		shared<Asset> LoadAsset(const string& Path, MapType& Cache, LoadFunc&& Loader);

		template<typename MapType>
		void GarbageCollect(MapType& Cache);

		static unique<AssetManager> UAssetManager;
		Dictionary<std::string, shared<sf::Texture>> LoadedTextures;
		Dictionary<string, shared<sf::Font>> LoadedFonts;
		std::string RootDirectory;
	};

	template<typename Asset, typename MapType, typename LoadFunc>
	inline shared<Asset> AssetManager::LoadAsset(const string& Path, MapType& Cache, LoadFunc&& Loader)
	{
		auto it = Cache.find(Path);
		if (it != Cache.end())
			return it->second;

		auto asset = std::make_shared<Asset>();

		if (Loader(*asset, RootDirectory + Path))
		{
			Cache.emplace(Path, asset);
			return asset;
		}

		return nullptr;
	}

	template<typename MapType>
	inline void AssetManager::GarbageCollect(MapType& Cache)
	{
		for (auto it = Cache.begin(); it != Cache.end();)
		{
			if (it->second.unique())
			{
				it = Cache.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
}