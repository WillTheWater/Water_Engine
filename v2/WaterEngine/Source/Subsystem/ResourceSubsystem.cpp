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
		// Start background cleanup thread
		CleanupThread = std::thread(&ResourceSubsystem::CleanupLoop, this);
	}

	ResourceSubsystem::~ResourceSubsystem()
	{
	}

	void ResourceSubsystem::Shutdown()
	{
		bRunning = false;
		DeletionCV.notify_all(); // Wake up thread to exit

		if (CleanupThread.joinable())
		{
			LOG("Thread Wakened")
			CleanupThread.join();
		}

		// Process deletions on shutdown (synchronous)
		ProcessDeferredFlush(std::numeric_limits<size_t>::max());
		LOG("Shutdown Clean")
	}

	void ResourceSubsystem::CleanupLoop()
	{
		while (bRunning)
		{
			std::unique_lock<std::mutex> Lock(CVMutex);

			// Shutdown signal
			DeletionCV.wait(Lock, [this]() {
				return !DeferredFlush.empty() || !bRunning;
				});

			// Process deletions
			size_t Count = 0;
			while (!DeferredFlush.empty() && bRunning)
			{
				DeferredFlush.front()();
				DeferredFlush.pop();
				Count++;
			}

			if (Count > 0)
			{
				LOG("Background thread unloaded {} resources", Count);
			}
		}
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
				PersistentBuffer = Data;
				return Fnt.openFromMemory(PersistentBuffer.data(), PersistentBuffer.size());
			}
		);
	}

	shared<tgui::Font> ResourceSubsystem::LoadTGUIFont(const string& Path)
	{
		auto it = LoadedTGUIFonts.find(Path);
		if (it != LoadedTGUIFonts.end()) { return make_shared<tgui::Font>(it->second); }

		if (!AssetDirectory) { return nullptr; }

		list<uint8> FileData;
		if (!AssetDirectory->ReadFile(Path, FileData)) { return nullptr; }

		// Convert to vector for contiguous memory
		list<uint8> Buffer(FileData.begin(), FileData.end());

		// Construct TGUI font from memory
		tgui::Font NewFont(Buffer.data(), Buffer.size());
		auto Result = make_shared<tgui::Font>(NewFont);

		// Store copy for reuse
		LoadedTGUIFonts[Path] = NewFont;

		return Result;
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

	void ResourceSubsystem::ProcessDeferredFlush(size_t MaxPerFrame)
	{
		std::lock_guard<std::mutex> Lock(FlushMutex);

		size_t Processed = 0;
		while (!DeferredFlush.empty() && Processed < MaxPerFrame)
		{
			DeferredFlush.front()();
			DeferredFlush.pop();
			++Processed;
			LOG("Processed: {}", Processed)
		}
	}
}