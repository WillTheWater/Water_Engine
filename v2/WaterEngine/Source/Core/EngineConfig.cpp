// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Core/EngineConfig.h"
#include "Utility/Log.h"
#include <physfs.h>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace we
{
	EngineConfig EngineConfigManager::CurrentConfig;
	string EngineConfigManager::LoadedPath;
	bool EngineConfigManager::IsLoaded = false;

	static std::string ReadFileContent(const string& FilePath)
	{
		if (PHYSFS_isInit())
		{
			PHYSFS_File* File = PHYSFS_openRead(FilePath.c_str());
			if (File)
			{
				PHYSFS_sint64 Size = PHYSFS_fileLength(File);
				if (Size > 0)
				{
					std::string Content(Size, '\0');
					PHYSFS_readBytes(File, Content.data(), Size);
					PHYSFS_close(File);
					return Content;
				}
				PHYSFS_close(File);
			}
		}

		std::ifstream File(FilePath);
		if (File)
		{
			std::stringstream Buffer;
			Buffer << File.rdbuf();
			return Buffer.str();
		}

		LOG("Failed to read file: {}", FilePath);
		return "";
	}

	void EngineConfigManager::ParseConfigTable(const toml::table& tbl)
	{
		if (auto window = tbl["Window"])
		{
			CurrentConfig.Window.Title = window["Title"].value_or("Water Engine");
			CurrentConfig.Window.Fullscreen = window["Fullscreen"].value_or(false);
			CurrentConfig.Window.VSync = window["VSync"].value_or(true);
			CurrentConfig.Window.TargetFPS = window["TargetFPS"].value_or(60.0);

			if (auto arr = window["DefaultSize"].as_array())
			{
				if (arr->size() >= 2)
				{
					CurrentConfig.Window.DefaultSize.x = arr->get(0)->value_or(1920u);
					CurrentConfig.Window.DefaultSize.y = arr->get(1)->value_or(1080u);
				}
			}

			if (auto arr = window["MinimumSize"].as_array())
			{
				if (arr->size() >= 2)
				{
					CurrentConfig.Window.MinimumSize.x = arr->get(0)->value_or(1280u);
					CurrentConfig.Window.MinimumSize.y = arr->get(1)->value_or(720u);
				}
			}
		}

		if (auto paths = tbl["Paths"])
		{
			CurrentConfig.Paths.AssetDirectory = paths["AssetDirectory"].value_or("Content");
		}

		if (auto physics = tbl["Physics"])
		{
			if (auto arr = physics["Gravity"].as_array())
			{
				if (arr->size() >= 2)
				{
					CurrentConfig.Physics.Gravity.x = arr->get(0)->value_or(0.0);
					CurrentConfig.Physics.Gravity.y = arr->get(1)->value_or(9.8);
				}
			}
			CurrentConfig.Physics.TimeStep = static_cast<float>(physics["TimeStep"].value_or(0.016));
		}

		if (auto audio = tbl["Audio"])
		{
			CurrentConfig.Audio.MasterVolume = static_cast<float>(audio["MasterVolume"].value_or(1.0));
			CurrentConfig.Audio.MusicVolume = static_cast<float>(audio["MusicVolume"].value_or(0.8));
			CurrentConfig.Audio.AmbientVolume = static_cast<float>(audio["AmbientVolume"].value_or(0.8));
			CurrentConfig.Audio.SFXVolume = static_cast<float>(audio["SFXVolume"].value_or(0.8));
			CurrentConfig.Audio.VoiceVolume = static_cast<float>(audio["VoiceVolume"].value_or(1.0));
			CurrentConfig.Audio.UIVolume = static_cast<float>(audio["UIVolume"].value_or(1.0));
			CurrentConfig.Audio.MaxSFXInstances = audio["MaxSFXInstances"].value_or(32u);
		}

		if (auto render = tbl["Render"])
		{
			CurrentConfig.Render.ResolutionX = render["ResolutionX"].value_or(1920u);
			CurrentConfig.Render.ResolutionY = render["ResolutionY"].value_or(1080u);
			CurrentConfig.Render.WorldSmooth = render["WorldSmooth"].value_or(true);
			CurrentConfig.Render.WorldUISmooth = render["WorldUISmooth"].value_or(false);
			CurrentConfig.Render.ScreenUISmooth = render["ScreenUISmooth"].value_or(false);
			CurrentConfig.Render.CursorSmooth = render["CursorSmooth"].value_or(false);
		}

		if (auto cursor = tbl["Cursor"])
		{
			if (auto arr = cursor["CursorSize"].as_array())
			{
				if (arr->size() >= 2)
				{
					CurrentConfig.Cursor.CursorSize.x = arr->get(0)->value_or(32u);
					CurrentConfig.Cursor.CursorSize.y = arr->get(1)->value_or(32u);
				}

			}
			CurrentConfig.Cursor.CursorSpeed = cursor["CursorSpeed"].value_or(100);
			CurrentConfig.Cursor.JoystickDeadzone = cursor["JoystickDeadzone"].value_or(0.15);
		}
	}

	bool EngineConfigManager::LoadFromFile(const string& FilePath)
	{
		try
		{
			std::string Content = ReadFileContent(FilePath);
			if (Content.empty())
			{
				LOG("Empty config file: {}", FilePath);
				return false;
			}

			ParseConfigTable(toml::parse(Content));

			LoadedPath = FilePath;
			IsLoaded = true;
			return true;
		}
		catch (const toml::parse_error& e)
		{
			LOG("TOML parse error in {}: {}", FilePath, e.what());
			return false;
		}
		catch (const std::exception& e)
		{
			LOG("Error loading config {}: {}", FilePath, e.what());
			return false;
		}
	}

	bool EngineConfigManager::Reload()
	{
		if (LoadedPath.empty())
			return false;

		if (LoadedPath.find("pak://") == 0)
		{
			// Handle reloading from asset director if needed, 
			// though usually re-pointing to LoadFromFile is standard.
			return false;
		}

		return LoadFromFile(LoadedPath);
	}

	bool EngineConfigManager::LoadFromAssetDirector(const IAssetDirector& AssetDir, const string& FilePath)
	{
		try
		{
			std::vector<uint8_t> Data;
			if (!AssetDir.ReadFile(FilePath, Data))
			{
				LOG("Failed to read config from AssetDirector: {}", FilePath);
				return false;
			}

			std::string Content(reinterpret_cast<const char*>(Data.data()), Data.size());
			if (Content.empty())
			{
				LOG("Empty config file in AssetDirector: {}", FilePath);
				return false;
			}

			ParseConfigTable(toml::parse(Content));

			LoadedPath = "pak://" + FilePath;
			IsLoaded = true;
			return true;
		}
		catch (const toml::parse_error& e)
		{
			LOG("TOML parse error in {}: {}", FilePath, e.what());
			return false;
		}
		catch (const std::exception& e)
		{
			LOG("Error loading config {}: {}", FilePath, e.what());
			return false;
		}
	}
}