// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/WaterEngine.h"
#include "Utility/Log.h"
#include "Asset/FileWatcher.h"
#include "Asset/PakSource.h"
#include <toml++/toml.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <physfs.h>
#include <filesystem>

namespace we
{
	static FileWatcher TextureWatcher;
	static string WatchedTexturePath;
	static bool NeedsReload = false;

	static float SpriteRotation = 0.0f;
	static bool ShowEditor = true;
	static bool ImGuiInitialized = false;

	WaterEngine::WaterEngine()
		: Running(true), Initialized(false)
	{
	}

	WaterEngine::~WaterEngine() = default;

	bool WaterEngine::IsRunning() const
	{
		return Running;
	}

	bool WaterEngine::Init()
	{
		if (Initialized)
			return true;

		#ifdef WE_RELEASE
		if (std::filesystem::exists("Content.pak"))
		{
			LOG("Found Content.pak, mounting...");
			if (PakSource::MountPak("Content.pak", "/"))
			{
				LOG("Content.pak mounted successfully");
			}
			else
			{
				LOG("Failed to mount Content.pak");
			}
		}
		#endif

		#ifdef WE_RELEASE
		const char* ConfigPath = "Content/Config/EngineConfig.ini";
		#else
		const char* ConfigPath = "EngineConfig.ini";
		#endif
		
		if (!EngineConfigManager::LoadFromFile(ConfigPath))
		{
			LOG("Failed to load {}", ConfigPath);
			return false;
		}

		const EngineConfig& Config = EngineConfigManager::Get();
		sf::VideoMode VideoMode({ Config.Window.Width, Config.Window.Height });
		Window = std::make_unique<sf::RenderWindow>(VideoMode, Config.Window.Title);
		Window->setVerticalSyncEnabled(Config.Window.VSync);
		LOG("Window created: {}x{} - {}", Config.Window.Width, Config.Window.Height, Config.Window.Title);

		if (!ImGui::SFML::Init(*Window))
		{
			LOG("Failed to initialize ImGui-SFML");
			return false;
		}
		ImGuiInitialized = true;

		#ifdef WE_RELEASE
		ImGui::GetIO().IniFilename = nullptr;
		#endif

		string TexturePath = "Content/Assets/Textures/Default/defaultCursor.png";
		if (!LoadTextureDirect(TexturePath))
		{
			TexturePath = "./Content/Assets/Textures/Default/defaultCursor.png";
		}
		if (LoadTextureDirect(TexturePath))
		{
			LOG("Loaded texture: {}", TexturePath);
			WatchedTexturePath = TexturePath;

			#ifndef WE_RELEASE
			TextureWatcher.WatchDirectory("Content/Assets/Textures/Default");
			TextureWatcher.SetCallback([](const string& FileName)
			{
				if (FileName.find("defaultCursor") != string::npos)
				{
					LOG("Texture changed: {}", FileName);
					NeedsReload = true;
				}
			});
			#endif
		}

		LoadRotation();

		Initialized = true;
		return true;
	}

	void WaterEngine::TestRun()
	{
		if (!Initialized)
		{
			if (!Init())
			{
				Running = false;
				return;
			}
		}

		#ifndef WE_RELEASE
		TextureWatcher.Update();
		if (NeedsReload)
		{
			NeedsReload = false;
			TestTextures.erase(WatchedTexturePath);
			if (LoadTextureDirect(WatchedTexturePath))
			{
				LOG("Texture hot reloaded!");
			}
		}
		#endif

		static sf::Clock DeltaClock;
		ImGui::SFML::Update(*Window, DeltaClock.restart());

		Window->clear(sf::Color::Black);

		auto It = TestTextures.find(WatchedTexturePath);
		if (It != TestTextures.end())
		{
			sf::Sprite Sprite(It->second);
			auto Bounds = Sprite.getLocalBounds();
			Sprite.setOrigin(sf::Vector2f(Bounds.size.x / 2.0f, Bounds.size.y / 2.0f));
			Sprite.setPosition(sf::Vector2f(400.0f, 300.0f));
			Sprite.setRotation(sf::degrees(SpriteRotation));
			Window->draw(Sprite);
		}

		if (ShowEditor)
		{
			ImGui::Begin("Editor");
			
			ImGui::Text("Cursor Sprite");
			ImGui::SliderFloat("Rotation", &SpriteRotation, 0.0f, 360.0f);
			
			if (ImGui::Button("Save"))
			{
				SaveRotation();
				LOG("Rotation saved: {}", SpriteRotation);
			}
			ImGui::SameLine();
			if (ImGui::Button("Load"))
			{
				LoadRotation();
				LOG("Rotation loaded: {}", SpriteRotation);
			}
			
			ImGui::End();
		}

		ImGui::SFML::Render(*Window);

		Window->display();

		while (const auto Event = Window->pollEvent())
		{
			ImGui::SFML::ProcessEvent(*Window, *Event);
			
			if (Event->is<sf::Event::Closed>())
				Running = false;
		}
	}

	void WaterEngine::ReloadConfig()
	{
		if (EngineConfigManager::Reload())
		{
			const EngineConfig& Config = EngineConfigManager::Get();
			LOG("Config reloaded! VSync: {}", Config.Window.VSync);
			
			if (Window)
			{
				Window->setVerticalSyncEnabled(Config.Window.VSync);
			}
		}
		else
		{
			LOG("Failed to reload config");
		}
	}

	sf::Texture* WaterEngine::LoadTextureDirect(const string& Path)
	{
		sf::Texture Texture;
		
		if (PHYSFS_isInit())
		{
			PHYSFS_File* File = PHYSFS_openRead(Path.c_str());
			if (File)
			{
				PHYSFS_sint64 Size = PHYSFS_fileLength(File);
				if (Size > 0)
				{
					std::vector<uint8> Buffer(Size);
					PHYSFS_readBytes(File, Buffer.data(), Size);
					PHYSFS_close(File);
					
					if (Texture.loadFromMemory(Buffer.data(), Buffer.size()))
					{
						string Name = Path;
						TestTextures[Name] = std::move(Texture);
						return &TestTextures[Name];
					}
				}
				PHYSFS_close(File);
			}
		}
		
		if (!Texture.loadFromFile(Path))
		{
			LOG("Failed to load texture: {}", Path);
			return nullptr;
		}

		string Name = Path;
		TestTextures[Name] = std::move(Texture);
		return &TestTextures[Name];
	}

	const EngineConfig& WaterEngine::GetConfig() const
	{
		return EngineConfigManager::Get();
	}

	void WaterEngine::SaveRotation()
	{
		std::ofstream File("SpriteRotation.toml");
		if (File.is_open())
		{
			File << "# Sprite Editor Data\n";
			File << "[Sprite]\n";
			File << "Rotation = " << SpriteRotation << "\n";
			File.close();
		}
	}

	void WaterEngine::LoadRotation()
	{
		try
		{
			toml::table tbl = toml::parse_file("SpriteRotation.toml");
			if (toml::node_view sprite = tbl["Sprite"])
			{
				SpriteRotation = static_cast<float>(sprite["Rotation"].value_or(0.0));
			}
		}
		catch (...)
		{
		}
	}
}
