// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/ResourceSubsystem.h"

namespace we
{
	unique<ResourceSubsystem> ResourceSubsystem::AssetManager{ nullptr };

	ResourceSubsystem::ResourceSubsystem()
		: AssetDirectory{}
	{
	}
	
	ResourceSubsystem& ResourceSubsystem::Get()
	{
		if (!AssetManager)
		{
			AssetManager = std::move(unique<ResourceSubsystem>{new ResourceSubsystem});
		}
		return *AssetManager;
	}

	void ResourceSubsystem::SetAssetDirectory(shared<IAssetDirector> Directory)
	{
		AssetDirectory = Directory;
	}

	shared<texture> ResourceSubsystem::LoadTexture(const string& Path)
	{
		return LoadAsset<texture>(Path, LoadedTextures,
			[&Path](texture& Tex, const list<uint8>& Data)
			{
				bool success = Tex.loadFromMemory(Data.data(), Data.size());
				if (!success)
				{
					// If this triggers, the bytes were found, but SFML couldn't parse them
					ERROR("SFML could not parse texture data for: {}", Path);
					ERROR("Data size: {} bytes", Data.size());
				}
				return success;
			}
		);
	}

	shared<font> ResourceSubsystem::LoadFont(const string& Path)
	{
		return LoadAsset<font>(Path, LoadedFonts,
			[this, &Path](font& Fnt, const list<uint8>& Data)
			{
				// SFML 3 fonts REQUIRE a buffer to stay alive.
				// Stored in a persistent buffer cache.
				// Note: 'Data' is a reference to FileData in the template.

				auto& PersistentBuffer = FontBuffers[Path];
				PersistentBuffer = Data; // Copy the data to cache

				return Fnt.openFromMemory(PersistentBuffer.data(), PersistentBuffer.size());
			}
		);
	}

	shared<soundBuffer> ResourceSubsystem::LoadSound(const string& Path)
	{
		return LoadAsset<soundBuffer>(Path, LoadedSounds,
			[](soundBuffer& Snd, const list<uint8>& Data)
			{
				return Snd.loadFromMemory(Data.data(), Data.size());
			}
		);
	}

}