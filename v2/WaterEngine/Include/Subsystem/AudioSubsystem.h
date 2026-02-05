// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"

namespace we
{
	enum class AudioChannel
	{
		Music,
		Ambient,
		SFX,
		Count
	};

	class AudioSubsystem
	{
	public:
		AudioSubsystem();

		void PlayMusic(const string& Path, bool bLoop = true);
		void PlayAmbient(const string& Path, bool bLoop = true);
		void PlaySFX(const string& Path, bool bLoop = false);

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
		void MuteChannel(AudioChannel Channel, bool bMute);
		bool IsChannelMuted(AudioChannel Channel) const;

		float GetEffectiveVolume(AudioChannel Channel) const;

		// 3D Audio
		void SetListenerPosition(vec3f Position);
		vec3f GetListenerPosition() const;

		void SetListenerDirection(vec3f Direction);
		vec3f GetListenerDirection() const;

		void SetMaxSFXCount(size_t Count);
		size_t GetActiveSFXCount() const;

	private:
		void ApplyVolumes();
		void CleanupStoppedSFX();
		float CalculateEffectiveVolume(AudioChannel Channel) const;

		float GlobalVolume;
		float ChannelVolumes[static_cast<int>(AudioChannel::Count)];
		bool ChannelMuted[static_cast<int>(AudioChannel::Count)];

		shared<sf::Music> CurrentMusic;
		shared<sf::Music> CurrentAmbient;
		list<sf::Sound> ActiveSFX;
		size_t MaxSFXCount;
	};
}