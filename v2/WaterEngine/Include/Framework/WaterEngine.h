// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Core/EngineConfig.h"

namespace we
{
	class WaterEngine
	{
	public:
		WaterEngine();
		virtual ~WaterEngine();

		bool Init();
		void TestRun();
		bool IsRunning() const;

		void ReloadConfig();
		sf::Texture* LoadTextureDirect(const string& Path);
		const EngineConfig& GetConfig() const;

		void SaveRotation();
		void LoadRotation();

	private:
		std::unique_ptr<sf::RenderWindow> Window;
		bool Running;
		bool Initialized;

		dictionary<string, sf::Texture> TestTextures;
	};
}
