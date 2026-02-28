// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Core/EngineConfig.h"
#include "Utility/Log.h"
#include <toml++/toml.hpp>
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

			toml::table tbl = toml::parse(Content);
			
			if (toml::node_view window = tbl["Window"])
			{
				CurrentConfig.Window.Title = window["Title"].value_or("Water Engine");
				CurrentConfig.Window.Width = window["Width"].value_or(1920u);
				CurrentConfig.Window.Height = window["Height"].value_or(1080u);
				CurrentConfig.Window.Fullscreen = window["Fullscreen"].value_or(false);
				CurrentConfig.Window.VSync = window["VSync"].value_or(true);
				CurrentConfig.Window.TargetFPS = window["TargetFPS"].value_or(60.0);
			}

			if (toml::node_view paths = tbl["Paths"])
			{
				CurrentConfig.Paths.AssetDirectory = paths["AssetDirectory"].value_or("Content");
				CurrentConfig.Paths.DefaultFont = paths["DefaultFont"].value_or("Fonts/Default.ttf");
			}

			if (toml::node_view debug = tbl["Debug"])
			{
				CurrentConfig.Debug.EnableHotReload = debug["EnableHotReload"].value_or(true);
				CurrentConfig.Debug.EnableConsole = debug["EnableConsole"].value_or(true);
				CurrentConfig.Debug.LogLevel = debug["LogLevel"].value_or("debug");
			}

			if (toml::node_view physics = tbl["Physics"])
			{
				CurrentConfig.Physics.GravityX = static_cast<float>(physics["GravityX"].value_or(0.0));
				CurrentConfig.Physics.GravityY = static_cast<float>(physics["GravityY"].value_or(9.8));
				CurrentConfig.Physics.TimeStep = static_cast<float>(physics["TimeStep"].value_or(0.016));
			}

			if (toml::node_view audio = tbl["Audio"])
			{
				CurrentConfig.Audio.MasterVolume = static_cast<float>(audio["MasterVolume"].value_or(1.0));
				CurrentConfig.Audio.MusicVolume = static_cast<float>(audio["MusicVolume"].value_or(0.8));
				CurrentConfig.Audio.AmbientVolume = static_cast<float>(audio["AmbientVolume"].value_or(0.8));
				CurrentConfig.Audio.SFXVolume = static_cast<float>(audio["SFXVolume"].value_or(0.8));
				CurrentConfig.Audio.VoiceVolume = static_cast<float>(audio["VoiceVolume"].value_or(1.0));
				CurrentConfig.Audio.UIVolume = static_cast<float>(audio["UIVolume"].value_or(1.0));
				CurrentConfig.Audio.MaxSFXInstances = audio["MaxSFXInstances"].value_or(32u);
			}

			if (toml::node_view render = tbl["Render"])
			{
				CurrentConfig.Render.ResolutionX = render["ResolutionX"].value_or(1920u);
				CurrentConfig.Render.ResolutionY = render["ResolutionY"].value_or(1080u);
				CurrentConfig.Render.WorldSmooth = render["WorldSmooth"].value_or(true);
				CurrentConfig.Render.WorldUISmooth = render["WorldUISmooth"].value_or(false);
				CurrentConfig.Render.ScreenUISmooth = render["ScreenUISmooth"].value_or(false);
				CurrentConfig.Render.CursorSmooth = render["CursorSmooth"].value_or(false);
			}

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

			toml::table tbl = toml::parse(Content);
			
			if (toml::node_view window = tbl["Window"])
			{
				CurrentConfig.Window.Title = window["Title"].value_or("Water Engine");
				CurrentConfig.Window.Width = window["Width"].value_or(1920u);
				CurrentConfig.Window.Height = window["Height"].value_or(1080u);
				CurrentConfig.Window.Fullscreen = window["Fullscreen"].value_or(false);
				CurrentConfig.Window.VSync = window["VSync"].value_or(true);
				CurrentConfig.Window.TargetFPS = window["TargetFPS"].value_or(60.0);
			}

			if (toml::node_view paths = tbl["Paths"])
			{
				CurrentConfig.Paths.AssetDirectory = paths["AssetDirectory"].value_or("Content");
				CurrentConfig.Paths.DefaultFont = paths["DefaultFont"].value_or("Fonts/Default.ttf");
			}

			if (toml::node_view debug = tbl["Debug"])
			{
				CurrentConfig.Debug.EnableHotReload = debug["EnableHotReload"].value_or(true);
				CurrentConfig.Debug.EnableConsole = debug["EnableConsole"].value_or(true);
				CurrentConfig.Debug.LogLevel = debug["LogLevel"].value_or("debug");
			}

			if (toml::node_view physics = tbl["Physics"])
			{
				CurrentConfig.Physics.GravityX = static_cast<float>(physics["GravityX"].value_or(0.0));
				CurrentConfig.Physics.GravityY = static_cast<float>(physics["GravityY"].value_or(9.8));
				CurrentConfig.Physics.TimeStep = static_cast<float>(physics["TimeStep"].value_or(0.016));
			}

			if (toml::node_view audio = tbl["Audio"])
			{
				CurrentConfig.Audio.MasterVolume = static_cast<float>(audio["MasterVolume"].value_or(1.0));
				CurrentConfig.Audio.MusicVolume = static_cast<float>(audio["MusicVolume"].value_or(0.8));
				CurrentConfig.Audio.AmbientVolume = static_cast<float>(audio["AmbientVolume"].value_or(0.8));
				CurrentConfig.Audio.SFXVolume = static_cast<float>(audio["SFXVolume"].value_or(0.8));
				CurrentConfig.Audio.VoiceVolume = static_cast<float>(audio["VoiceVolume"].value_or(1.0));
				CurrentConfig.Audio.UIVolume = static_cast<float>(audio["UIVolume"].value_or(1.0));
				CurrentConfig.Audio.MaxSFXInstances = audio["MaxSFXInstances"].value_or(32u);
			}

			if (toml::node_view render = tbl["Render"])
			{
				CurrentConfig.Render.ResolutionX = render["ResolutionX"].value_or(1920u);
				CurrentConfig.Render.ResolutionY = render["ResolutionY"].value_or(1080u);
				CurrentConfig.Render.WorldSmooth = render["WorldSmooth"].value_or(true);
				CurrentConfig.Render.WorldUISmooth = render["WorldUISmooth"].value_or(false);
				CurrentConfig.Render.ScreenUISmooth = render["ScreenUISmooth"].value_or(false);
				CurrentConfig.Render.CursorSmooth = render["CursorSmooth"].value_or(false);
			}

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
