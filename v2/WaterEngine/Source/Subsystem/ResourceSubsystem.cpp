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

				auto& PersistentBuffer = FontBuffers[Path];

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