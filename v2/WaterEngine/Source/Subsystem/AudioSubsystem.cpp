// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/AudioSubsystem.h"
#include "EngineConfig.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Utility/Log.h"

namespace we
{
	AudioSubsystem::AudioSubsystem(const AudioConfig& InConfig)
		: GlobalVolume{ InConfig.StartupGlobalVolume }
		, ChannelVolumes{ 10, 20, 100 }
		, ChannelMuted{ false, false, false }
		, MaxSFXCount{ InConfig.MaxSFXStack }
		, Config{ InConfig }
	{
		sf::Listener::setGlobalVolume(Config.StartupGlobalVolume);
		sf::Listener::setPosition({ 0.f, 0.f, 0.f });
		sf::Listener::setDirection({ 0.f, 0.f, -1.f });
	}

	void AudioSubsystem::PlayMusic(const string& Path, bool bInLoop)
	{
		StopMusic();

		CurrentMusic = LoadAsset().LoadMusic(Path);
		if (CurrentMusic)
		{
			CurrentMusic->setLooping(bInLoop);
			CurrentMusic->setVolume(GetEffectiveVolume(AudioChannel::Music));
			CurrentMusic->play();
		}
	}

	void AudioSubsystem::PlayAmbient(const string& Path, bool bInLoop)
	{
		StopAmbient();

		CurrentAmbient = LoadAsset().LoadMusic(Path);
		if (CurrentAmbient)
		{
			CurrentAmbient->setLooping(bInLoop);
			CurrentAmbient->setVolume(GetEffectiveVolume(AudioChannel::Ambient));
			CurrentAmbient->play();
		}
	}

	void AudioSubsystem::PlaySFX(const string& Path, bool bInLoop)
	{
		auto SoundBuffer = LoadAsset().LoadSoundSync(Path);
		if (!SoundBuffer || SoundBuffer->getSampleCount() == 0)
		{
			ERROR("Failed to load sound: {}", Path);
			return;
		}

		CleanupStoppedSFX();

		// Remove oldest SFX if at limit
		while (ActiveSFX.size() >= MaxSFXCount && !ActiveSFX.empty())
		{
			ActiveSFX.erase(ActiveSFX.begin());
		}

		SFXInstance Instance;
		Instance.Buffer = SoundBuffer;
		Instance.Sound = make_unique<sf::Sound>(*SoundBuffer);
		Instance.Sound->setLooping(bInLoop);
		Instance.Sound->setVolume(GetEffectiveVolume(AudioChannel::SFX));
		Instance.Sound->play();

		ActiveSFX.push_back(std::move(Instance));
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
		for (auto& Instance : ActiveSFX)
		{
			if (Instance.Sound)
			{
				Instance.Sound->stop();
			}
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
		ChannelVolumes[static_cast<int>(Channel)] = std::clamp(Volume, 0.f, 100.f);
		ApplyVolumes();
	}

	float AudioSubsystem::GetChannelVolume(AudioChannel Channel) const
	{
		return ChannelVolumes[static_cast<int>(Channel)];
	}

	void AudioSubsystem::MuteChannel(AudioChannel Channel, bool bInMute)
	{
		ChannelMuted[static_cast<int>(Channel)] = bInMute;
		ApplyVolumes();
	}

	bool AudioSubsystem::IsChannelMuted(AudioChannel Channel) const
	{
		return ChannelMuted[static_cast<int>(Channel)];
	}

	float AudioSubsystem::GetEffectiveVolume(AudioChannel Channel) const
	{
		return CalculateEffectiveVolume(Channel);
	}

	void AudioSubsystem::SetListenerPosition(vec3f Position)
	{
		sf::Listener::setPosition({ Position.x, Position.y, Position.z });
	}

	vec3f AudioSubsystem::GetListenerPosition() const
	{
		auto Pos = sf::Listener::getPosition();
		return { Pos.x, Pos.y, Pos.z };
	}

	void AudioSubsystem::SetListenerDirection(vec3f Direction)
	{
		sf::Listener::setDirection({ Direction.x, Direction.y, Direction.z });
	}

	vec3f AudioSubsystem::GetListenerDirection() const
	{
		auto Dir = sf::Listener::getDirection();
		return { Dir.x, Dir.y, Dir.z };
	}

	void AudioSubsystem::SetMaxSFXCount(size_t Count)
	{
		MaxSFXCount = Count;
	}

	size_t AudioSubsystem::GetActiveSFXCount() const
	{
		return ActiveSFX.size();
	}

	void AudioSubsystem::Update()
	{
		CleanupStoppedSFX();
	}

	void AudioSubsystem::ApplyVolumes()
	{
		sf::Listener::setGlobalVolume(GlobalVolume);

		if (CurrentMusic)
		{
			CurrentMusic->setVolume(GetEffectiveVolume(AudioChannel::Music));
		}
		if (CurrentAmbient)
		{
			CurrentAmbient->setVolume(GetEffectiveVolume(AudioChannel::Ambient));
		}

		float SFXVolume = GetEffectiveVolume(AudioChannel::SFX);
		for (auto& Instance : ActiveSFX)
		{
			if (Instance.Sound && Instance.Sound->getStatus() == sf::Sound::Status::Playing)
			{
				Instance.Sound->setVolume(SFXVolume);
			}
		}
	}

	void AudioSubsystem::CleanupStoppedSFX()
	{
		ActiveSFX.erase(
			std::remove_if(ActiveSFX.begin(), ActiveSFX.end(),
				[](const SFXInstance& Instance)
				{
					return !Instance.Sound || Instance.Sound->getStatus() == sf::Sound::Status::Stopped;
				}),
			ActiveSFX.end()
		);
	}

	float AudioSubsystem::CalculateEffectiveVolume(AudioChannel Channel) const
	{
		if (IsChannelMuted(Channel)) return 0.f;

		float ChannelPct = GetChannelVolume(Channel) / 100.f;
		float GlobalPct = GlobalVolume / 100.f;

		return (ChannelPct * GlobalPct) * 100.f;
	}
}
