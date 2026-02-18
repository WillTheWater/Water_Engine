// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Core/AudioTypes.h"

namespace we
{
	struct AudioConfig
	{
		float StartupGlobalVolume;
		ulong MaxSFXStack;
	};

	enum class AudioChannel
	{
		Music,
		Ambient,
		SFX,
		Count
	};

	struct SFXInstance
	{
		shared<soundBuffer> Buffer;
		unique<sf::Sound> Sound;
	};

	class AudioSubsystem
	{
	public:
		explicit AudioSubsystem(const AudioConfig& Config);

		void PlayMusic(const string& Path, bool bInLoop = true);
		void PlayAmbient(const string& Path, bool bInLoop = true);
		void PlaySFX(const string& Path, bool bInLoop = false);

		void StopMusic();
		void StopAmbient();
		void StopSFX();
		void StopAll();

		// Global
		void SetGlobalVolume(float Volume);
		float GetGlobalVolume() const;

		// Channels
		void SetChannelVolume(AudioChannel Channel, float Volume);
		float GetChannelVolume(AudioChannel Channel) const;
		void MuteChannel(AudioChannel Channel, bool bInMute);
		bool IsChannelMuted(AudioChannel Channel) const;

		float GetEffectiveVolume(AudioChannel Channel) const;

		// 3D Audio
		void SetListenerPosition(vec3f Position);
		vec3f GetListenerPosition() const;

		void SetListenerDirection(vec3f Direction);
		vec3f GetListenerDirection() const;

		void SetMaxSFXCount(size_t Count);
		size_t GetActiveSFXCount() const;

		void Update();

	private:
		void ApplyVolumes();
		void CleanupStoppedSFX();
		float CalculateEffectiveVolume(AudioChannel Channel) const;

		float GlobalVolume;
		float ChannelVolumes[static_cast<int>(AudioChannel::Count)];
		bool ChannelMuted[static_cast<int>(AudioChannel::Count)];

		shared<music> CurrentMusic;
		shared<music> CurrentAmbient;
		vector<SFXInstance> ActiveSFX;
		size_t MaxSFXCount;
		AudioConfig Config;
	};
}