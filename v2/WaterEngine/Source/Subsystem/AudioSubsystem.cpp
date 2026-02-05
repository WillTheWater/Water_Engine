// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include <SFML/Audio/Listener.hpp>
#include "EngineConfig.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Subsystem/AudioSubsystem.h"
#include "Utility/Log.h"

namespace we
{
	AudioSubsystem::AudioSubsystem()
		: GlobalVolume{EC.StartupGlobalVolume}
		, ChannelVolumes{20, 70, 100} // Music | Ambient | SFX
		, ChannelMuted{ false, false, false }
		, MaxSFXCount{EC.MaxSFXStack}
	{
		sf::SoundBuffer Dummy;
		sf::Listener::setGlobalVolume(EC.StartupGlobalVolume);
		sf::Listener::setPosition({ 0.f, 0.f, 0.f });
		sf::Listener::setDirection({ 0.f, 0.f, -1.f });
	}

	void AudioSubsystem::PlayMusic(const string& Path, bool bLoop)
	{
		StopMusic();

		CurrentMusic = Asset().LoadMusic(Path);
		if (CurrentMusic)
		{
			CurrentMusic->setLooping(bLoop);
			CurrentMusic->setVolume(GetChannelVolume(AudioChannel::Music));
			CurrentMusic->play();
		}
	}

	void AudioSubsystem::PlayAmbient(const string& Path, bool bLoop)
	{
		StopAmbient();

		CurrentAmbient = Asset().LoadMusic(Path);
		if (CurrentAmbient)
		{
			CurrentAmbient->setLooping(bLoop);
			CurrentAmbient->setVolume(GetChannelVolume(AudioChannel::Ambient));
			CurrentAmbient->play();
		}
	}

	void AudioSubsystem::PlaySFX(const string& Path, bool bLoop)
	{
		auto SoundBuffer = Asset().LoadSound(Path);
		if (!SoundBuffer)
		{
			ERROR("SoundBuffer is null: {}", Path);
			return;
		}

		if (SoundBuffer->getSampleCount() == 0)
		{
			ERROR("SoundBuffer has no samples: {}", Path);
			return;
		}

		CleanupStoppedSFX();

		while (ActiveSFX.size() >= MaxSFXCount && !ActiveSFX.empty())
		{
			ActiveSFX.erase(ActiveSFX.begin());
		}

		auto& NewSound = ActiveSFX.emplace_back(*SoundBuffer);
		NewSound.setLooping(bLoop);
		float Vol = GetEffectiveVolume(AudioChannel::SFX);

		NewSound.setVolume(Vol);
		NewSound.play();
	}

	void AudioSubsystem::StopMusic()
	{
		if (CurrentMusic)
		{
			CurrentMusic->stop();
			CurrentMusic = nullptr;
		}
	}

	void AudioSubsystem::StopAmbient()
	{
		if (CurrentAmbient)
		{
			CurrentAmbient->stop();
			CurrentAmbient = nullptr;
		}
	}

	void AudioSubsystem::StopSFX()
	{
		for (auto& Sound : ActiveSFX)
		{
			Sound.stop();
		}
		ActiveSFX.clear();
	}

	void AudioSubsystem::StopAll()
	{
		StopMusic();
		StopAmbient();
		StopSFX();
	}

	void AudioSubsystem::SetGlobalVolume(float Volume)
	{
		GlobalVolume = std::clamp(Volume, 0.f, 100.f);
		ApplyVolumes();
	}

	float AudioSubsystem::GetGlobalVolume() const
	{
		return GlobalVolume;
	}

	void AudioSubsystem::SetChannelVolume(AudioChannel Channel, float Volume)
	{
		int Index = static_cast<int>(Channel);
		if (Index >= 0 && Index < static_cast<int>(AudioChannel::Count))
		{
			ChannelVolumes[Index] = std::clamp(Volume, 0.f, 100.f);
			ApplyVolumes();
		}
	}

	float AudioSubsystem::GetChannelVolume(AudioChannel Channel) const
	{
		int Index = static_cast<int>(Channel);
		if (Index >= 0 && Index < static_cast<int>(AudioChannel::Count))
		{
			return ChannelVolumes[Index];
		}
		return 100.f;
	}

	void AudioSubsystem::MuteChannel(AudioChannel Channel, bool bMute)
	{
		int Index = static_cast<int>(Channel);
		if (Index >= 0 && Index < static_cast<int>(AudioChannel::Count))
		{
			ChannelMuted[Index] = bMute;
			ApplyVolumes();
		}
	}

	bool AudioSubsystem::IsChannelMuted(AudioChannel Channel) const
	{
		int Index = static_cast<int>(Channel);
		if (Index >= 0 && Index < static_cast<int>(AudioChannel::Count))
		{
			return ChannelMuted[Index];
		}
		return false;
	}

	void AudioSubsystem::ApplyVolumes()
	{
		if (CurrentMusic)
		{
			CurrentMusic->setVolume(GetEffectiveVolume(AudioChannel::Music));
		}
		if (CurrentAmbient)
		{
			CurrentAmbient->setVolume(GetEffectiveVolume(AudioChannel::Ambient));
		}

		float SFXVolume = GetEffectiveVolume(AudioChannel::SFX);
		for (auto& Sound : ActiveSFX)
		{
			if (Sound.getStatus() == sf::Sound::Status::Playing)
			{
				Sound.setVolume(SFXVolume);
			}
		}
	}

	float AudioSubsystem::GetEffectiveVolume(AudioChannel Channel) const
	{
		return CalculateEffectiveVolume(Channel);
	}

	float AudioSubsystem::CalculateEffectiveVolume(AudioChannel Channel) const
	{
		if (IsChannelMuted(Channel)) return 0.f;

		float ChannelPct = GetChannelVolume(Channel) / 100.f;
		float GlobalPct = GlobalVolume / 100.f;

		return (ChannelPct * GlobalPct) * 100.f;
	}

	void AudioSubsystem::CleanupStoppedSFX()
	{
		ActiveSFX.erase(
			std::remove_if(ActiveSFX.begin(), ActiveSFX.end(),
				[](const sf::Sound& s)
				{
					return s.getStatus() == sf::Sound::Status::Stopped;
				}),
			ActiveSFX.end()
		);
	}

	void AudioSubsystem::SetListenerPosition(vec3f Position)
	{
		sf::Listener::setPosition(Position);
	}

	vec3f AudioSubsystem::GetListenerPosition() const
	{
		return sf::Listener::getPosition();
	}

	void AudioSubsystem::SetListenerDirection(vec3f Direction)
	{
		sf::Listener::setDirection(Direction);
	}

	vec3f AudioSubsystem::GetListenerDirection() const
	{
		return sf::Listener::getDirection();
	}

	size_t AudioSubsystem::GetActiveSFXCount() const
	{
		return ActiveSFX.size();
	}

	void AudioSubsystem::SetMaxSFXCount(size_t Count)
	{
		MaxSFXCount = std::max(size_t(1), Count);
	}
}