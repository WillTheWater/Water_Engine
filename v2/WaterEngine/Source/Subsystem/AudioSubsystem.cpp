// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/AudioSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Utility/Log.h"
#include <cmath>

namespace we
{
    // ========================================================================
    // Constructor / Destructor
    // ========================================================================

    AudioSubsystem::AudioSubsystem(const EngineConfig::AudioConfig& InConfig, ResourceSubsystem& InResources)
        : Resources(InResources)
        , Config(InConfig)
        , MasterVolume(InConfig.MasterVolume)
    {
        // Initialize channel volumes
        ChannelVolumes[static_cast<size_t>(AudioChannel::Master)] = 1.0f;
        ChannelVolumes[static_cast<size_t>(AudioChannel::Music)] = InConfig.MusicVolume;
        ChannelVolumes[static_cast<size_t>(AudioChannel::Ambient)] = InConfig.AmbientVolume;
        ChannelVolumes[static_cast<size_t>(AudioChannel::SFX)] = InConfig.SFXVolume;
        ChannelVolumes[static_cast<size_t>(AudioChannel::Voice)] = InConfig.VoiceVolume;
        ChannelVolumes[static_cast<size_t>(AudioChannel::UI)] = InConfig.UIVolume;

        ChannelMuted.fill(false);

        // Setup SFML listener
        sf::Listener::setGlobalVolume(100.0f);  // We handle volume ourselves
        sf::Listener::setPosition({ 0.0f, 0.0f, 0.0f });
        sf::Listener::setDirection({ 0.0f, 0.0f, -1.0f });

        LOG("AudioSubsystem initialized");
    }

    AudioSubsystem::~AudioSubsystem() = default;

    // ========================================================================
    // Update
    // ========================================================================

    void AudioSubsystem::Update(float DeltaTime)
    {
        UpdateFades(DeltaTime);
        UpdateSpatialAudio();
        CleanupStoppedSounds();
    }

    // ========================================================================
    // Playback
    // ========================================================================

    void AudioSubsystem::PlayMusic(const string& Path, const AudioPlaybackConfig& InConfig)
    {
        StopMusic();

        // Load raw data
        auto data = Resources.LoadRawDataSync(Path);
        if (!data || data->empty())
        {
            ERROR("Failed to load music: {}", Path);
            return;
        }

        // Create music with memory stream
        CurrentMusic = we::make_unique<MusicTrack>();
        CurrentMusic->DataBuffer = data;
        CurrentMusic->Stream = we::make_unique<MusicMemoryStream>(data);
        CurrentMusic->Config = InConfig;
        CurrentMusic->Music = we::make_shared<music>();

        if (!CurrentMusic->Music->openFromStream(*CurrentMusic->Stream))
        {
            ERROR("Failed to open music stream: {}", Path);
            CurrentMusic = nullptr;
            return;
        }

        CurrentMusic->Music->setLooping(InConfig.bLoop);
        CurrentMusic->Music->setPitch(InConfig.Pitch);
        CurrentMusic->CurrentVolume = InConfig.Volume;
        CurrentMusic->TargetVolume = InConfig.Volume;
        
        ApplyVolumeToMusic(*CurrentMusic);
        CurrentMusic->Music->play();

        LOG("Playing music: {}", Path);
    }

    void AudioSubsystem::PlayAmbient(const string& Path, const AudioPlaybackConfig& InConfig)
    {
        StopAmbient();

        auto data = Resources.LoadRawDataSync(Path);
        if (!data || data->empty())
        {
            ERROR("Failed to load ambient: {}", Path);
            return;
        }

        CurrentAmbient = we::make_unique<MusicTrack>();
        CurrentAmbient->DataBuffer = data;
        CurrentAmbient->Stream = we::make_unique<MusicMemoryStream>(data);
        CurrentAmbient->Config = InConfig;
        CurrentAmbient->Music = we::make_shared<music>();

        if (!CurrentAmbient->Music->openFromStream(*CurrentAmbient->Stream))
        {
            ERROR("Failed to open ambient stream: {}", Path);
            CurrentAmbient = nullptr;
            return;
        }

        CurrentAmbient->Music->setLooping(InConfig.bLoop);
        CurrentAmbient->Music->setPitch(InConfig.Pitch);
        CurrentAmbient->CurrentVolume = InConfig.Volume;
        CurrentAmbient->TargetVolume = InConfig.Volume;
        
        ApplyVolumeToMusic(*CurrentAmbient);
        CurrentAmbient->Music->play();
    }

    void AudioSubsystem::CrossfadeMusic(const string& Path, float FadeDuration, const AudioPlaybackConfig& InConfig)
    {
        if (CurrentMusic && CurrentMusic->Music && CurrentMusic->Music->getStatus() == music::Status::Playing)
        {
            // Fade out current
            CurrentMusic->TargetVolume = 0.0f;
            CurrentMusic->FadeSpeed = CurrentMusic->CurrentVolume / FadeDuration;
        }

        // Start new music at 0 volume, fade in
        PlayMusic(Path, InConfig);
        if (CurrentMusic)
        {
            CurrentMusic->CurrentVolume = 0.0f;
            CurrentMusic->TargetVolume = InConfig.Volume;
            CurrentMusic->FadeSpeed = InConfig.Volume / FadeDuration;
            ApplyVolumeToMusic(*CurrentMusic);
        }
    }

    void AudioSubsystem::PlaySFX(const string& Path, const AudioPlaybackConfig& InConfig)
    {
        auto buffer = Resources.LoadSoundSync(Path);
        if (!buffer)
        {
            ERROR("Failed to load SFX: {}", Path);
            return;
        }

        CleanupStoppedSounds();

        // Remove oldest if at limit
        while (ActiveSFX.size() >= Config.MaxSFXInstances && !ActiveSFX.empty())
        {
            ActiveSFX.erase(ActiveSFX.begin());
        }

        SFXInstance instance;
        instance.Buffer = buffer;
        instance.Sound = we::make_unique<sound>(*buffer);
        instance.Config = InConfig;
        instance.CurrentVolume = InConfig.Volume;

        instance.Sound->setLooping(InConfig.bLoop);
        instance.Sound->setPitch(InConfig.Pitch);
        
        // Set 3D position if spatial
        if (InConfig.bSpatial)
        {
            instance.Sound->setPosition({ InConfig.Position.x, InConfig.Position.y, 0.0f });
            instance.Sound->setMinDistance(InConfig.MinDistance);
            instance.Sound->setAttenuation(DistanceAttenuationFactor);
        }

        ApplyVolumeToSFX(instance);
        instance.Sound->play();

        ActiveSFX.push_back(std::move(instance));
    }

    void AudioSubsystem::PlayVoice(const string& Path, const AudioPlaybackConfig& InConfig)
    {
        auto buffer = Resources.LoadSoundSync(Path);
        if (!buffer)
        {
            ERROR("Failed to load voice: {}", Path);
            return;
        }

        CleanupStoppedSounds();

        // Remove oldest voice if at limit
        while (ActiveVoice.size() >= MaxVoiceInstances && !ActiveVoice.empty())
        {
            ActiveVoice.erase(ActiveVoice.begin());
        }

        SFXInstance instance;
        instance.Buffer = buffer;
        instance.Sound = we::make_unique<sound>(*buffer);
        instance.Config = InConfig;
        instance.CurrentVolume = InConfig.Volume;

        instance.Sound->setLooping(false);  // Voice never loops
        instance.Sound->setPitch(InConfig.Pitch);
        
        ApplyVolumeToSFX(instance);
        instance.Sound->play();

        ActiveVoice.push_back(std::move(instance));
    }

    void AudioSubsystem::PlayUI(const string& Path, const AudioPlaybackConfig& InConfig)
    {
        // UI sounds bypass pause and use UI channel
        auto buffer = Resources.LoadSoundSync(Path);
        if (!buffer)
        {
            ERROR("Failed to load UI sound: {}", Path);
            return;
        }

        auto sound = we::make_unique<we::sound>(*buffer);
        
        // Calculate UI volume (ignores pause)
        Volume effectiveVol = InConfig.Volume;
        if (bMasterMuted || ChannelMuted[static_cast<size_t>(AudioChannel::UI)])
            effectiveVol = 0.0f;
        else
            effectiveVol *= MasterVolume * ChannelVolumes[static_cast<size_t>(AudioChannel::UI)];
        
        sound->setVolume(effectiveVol * 100.0f);
        sound->setPitch(InConfig.Pitch);
        sound->play();
        
        // UI sounds are fire-and-forget, we don't track them
        // The sound object will stop and cleanup automatically when destroyed
    }

    // ========================================================================
    // Stop
    // ========================================================================

    void AudioSubsystem::StopMusic(float FadeDuration)
    {
        if (!CurrentMusic || !CurrentMusic->Music)
            return;

        if (FadeDuration > 0.0f && CurrentMusic->Music->getStatus() == music::Status::Playing)
        {
            CurrentMusic->TargetVolume = 0.0f;
            CurrentMusic->FadeSpeed = CurrentMusic->CurrentVolume / FadeDuration;
        }
        else
        {
            CurrentMusic->Music->stop();
            CurrentMusic = nullptr;
        }
    }

    void AudioSubsystem::StopAmbient(float FadeDuration)
    {
        if (!CurrentAmbient || !CurrentAmbient->Music)
            return;

        if (FadeDuration > 0.0f && CurrentAmbient->Music->getStatus() == music::Status::Playing)
        {
            CurrentAmbient->TargetVolume = 0.0f;
            CurrentAmbient->FadeSpeed = CurrentAmbient->CurrentVolume / FadeDuration;
        }
        else
        {
            CurrentAmbient->Music->stop();
            CurrentAmbient = nullptr;
        }
    }

    void AudioSubsystem::StopAllSFX()
    {
        for (auto& sfx : ActiveSFX)
        {
            if (sfx.Sound)
                sfx.Sound->stop();
        }
        ActiveSFX.clear();
    }

    void AudioSubsystem::StopAllVoice()
    {
        for (auto& voice : ActiveVoice)
        {
            if (voice.Sound)
                voice.Sound->stop();
        }
        ActiveVoice.clear();
    }

    void AudioSubsystem::StopAll()
    {
        StopMusic();
        StopAmbient();
        StopAllSFX();
        StopAllVoice();
    }

    // ========================================================================
    // Volume Control
    // ========================================================================

    void AudioSubsystem::SetChannelVolume(AudioChannel Channel, Volume Vol)
    {
        ChannelVolumes[static_cast<size_t>(Channel)] = std::clamp(Vol, 0.0f, 1.0f);
    }

    Volume AudioSubsystem::GetChannelVolume(AudioChannel Channel) const
    {
        return ChannelVolumes[static_cast<size_t>(Channel)];
    }

    void AudioSubsystem::SetMasterVolume(Volume Vol)
    {
        MasterVolume = std::clamp(Vol, 0.0f, 1.0f);
    }

    Volume AudioSubsystem::GetMasterVolume() const
    {
        return MasterVolume;
    }

    // ========================================================================
    // Mute
    // ========================================================================

    void AudioSubsystem::SetChannelMuted(AudioChannel Channel, bool bMuted)
    {
        ChannelMuted[static_cast<size_t>(Channel)] = bMuted;
    }

    bool AudioSubsystem::IsChannelMuted(AudioChannel Channel) const
    {
        return ChannelMuted[static_cast<size_t>(Channel)];
    }

    void AudioSubsystem::SetMasterMuted(bool bMuted)
    {
        bMasterMuted = bMuted;
    }

    bool AudioSubsystem::IsMasterMuted() const
    {
        return bMasterMuted;
    }

    // ========================================================================
    // Pause
    // ========================================================================

    void AudioSubsystem::SetPaused(bool bInPaused)
    {
        if (bPaused == bInPaused)
            return;

        bPaused = bInPaused;

        if (bPaused)
        {
            // Pause music and ambient
            if (CurrentMusic && CurrentMusic->Music)
                CurrentMusic->Music->pause();
            if (CurrentAmbient && CurrentAmbient->Music)
                CurrentAmbient->Music->pause();
            
            // Pause SFX and voice
            for (auto& sfx : ActiveSFX)
                if (sfx.Sound) sfx.Sound->pause();
            for (auto& voice : ActiveVoice)
                if (voice.Sound) voice.Sound->pause();
        }
        else
        {
            // Resume
            if (CurrentMusic && CurrentMusic->Music)
                CurrentMusic->Music->play();
            if (CurrentAmbient && CurrentAmbient->Music)
                CurrentAmbient->Music->play();
            
            for (auto& sfx : ActiveSFX)
                if (sfx.Sound) sfx.Sound->play();
            for (auto& voice : ActiveVoice)
                if (voice.Sound) voice.Sound->play();
        }
    }

    bool AudioSubsystem::IsPaused() const
    {
        return bPaused;
    }

    // ========================================================================
    // 3D Audio
    // ========================================================================

    void AudioSubsystem::SetListenerPosition(vec2f Position)
    {
        ListenerPosition = Position;
        sf::Listener::setPosition({ Position.x, Position.y, 0.0f });
    }

    vec2f AudioSubsystem::GetListenerPosition() const
    {
        return ListenerPosition;
    }

    void AudioSubsystem::SetListenerDirection(vec2f Direction)
    {
        ListenerDirection = Direction;
        // SFML uses Z as forward, we use Y as down on screen
        sf::Listener::setDirection({ Direction.x, Direction.y, -1.0f });
    }

    vec2f AudioSubsystem::GetListenerDirection() const
    {
        return ListenerDirection;
    }

    // ========================================================================
    // Queries
    // ========================================================================

    bool AudioSubsystem::IsMusicPlaying() const
    {
        return CurrentMusic && CurrentMusic->Music && 
               CurrentMusic->Music->getStatus() == music::Status::Playing;
    }

    bool AudioSubsystem::IsAmbientPlaying() const
    {
        return CurrentAmbient && CurrentAmbient->Music && 
               CurrentAmbient->Music->getStatus() == music::Status::Playing;
    }

    size_t AudioSubsystem::GetActiveSFXCount() const
    {
        return ActiveSFX.size();
    }

    size_t AudioSubsystem::GetActiveVoiceCount() const
    {
        return ActiveVoice.size();
    }

    // ========================================================================
    // Private
    // ========================================================================

    void AudioSubsystem::UpdateFades(float DeltaTime)
    {
        // Update music fade
        if (CurrentMusic && CurrentMusic->FadeSpeed > 0.0f)
        {
            if (CurrentMusic->CurrentVolume < CurrentMusic->TargetVolume)
            {
                CurrentMusic->CurrentVolume += CurrentMusic->FadeSpeed * DeltaTime;
                if (CurrentMusic->CurrentVolume >= CurrentMusic->TargetVolume)
                {
                    CurrentMusic->CurrentVolume = CurrentMusic->TargetVolume;
                    CurrentMusic->FadeSpeed = 0.0f;
                    
                    // If faded out completely, stop
                    if (CurrentMusic->CurrentVolume <= 0.0f)
                    {
                        CurrentMusic->Music->stop();
                        CurrentMusic = nullptr;
                        return;
                    }
                }
                ApplyVolumeToMusic(*CurrentMusic);
            }
            else if (CurrentMusic->CurrentVolume > CurrentMusic->TargetVolume)
            {
                CurrentMusic->CurrentVolume -= CurrentMusic->FadeSpeed * DeltaTime;
                if (CurrentMusic->CurrentVolume <= CurrentMusic->TargetVolume)
                {
                    CurrentMusic->CurrentVolume = CurrentMusic->TargetVolume;
                    CurrentMusic->FadeSpeed = 0.0f;
                }
                ApplyVolumeToMusic(*CurrentMusic);
            }
        }

        // Update ambient fade
        if (CurrentAmbient && CurrentAmbient->FadeSpeed > 0.0f)
        {
            if (CurrentAmbient->CurrentVolume < CurrentAmbient->TargetVolume)
            {
                CurrentAmbient->CurrentVolume += CurrentAmbient->FadeSpeed * DeltaTime;
                if (CurrentAmbient->CurrentVolume >= CurrentAmbient->TargetVolume)
                {
                    CurrentAmbient->CurrentVolume = CurrentAmbient->TargetVolume;
                    CurrentAmbient->FadeSpeed = 0.0f;
                    
                    if (CurrentAmbient->CurrentVolume <= 0.0f)
                    {
                        CurrentAmbient->Music->stop();
                        CurrentAmbient = nullptr;
                        return;
                    }
                }
                ApplyVolumeToMusic(*CurrentAmbient);
            }
            else if (CurrentAmbient->CurrentVolume > CurrentAmbient->TargetVolume)
            {
                CurrentAmbient->CurrentVolume -= CurrentAmbient->FadeSpeed * DeltaTime;
                if (CurrentAmbient->CurrentVolume <= CurrentAmbient->TargetVolume)
                {
                    CurrentAmbient->CurrentVolume = CurrentAmbient->TargetVolume;
                    CurrentAmbient->FadeSpeed = 0.0f;
                }
                ApplyVolumeToMusic(*CurrentAmbient);
            }
        }
    }

    void AudioSubsystem::UpdateSpatialAudio()
    {
        // Update SFX spatial audio
        for (auto& sfx : ActiveSFX)
        {
            if (!sfx.Sound || !sfx.Config.bSpatial)
                continue;

            // Update position
            sfx.Sound->setPosition({ sfx.Config.Position.x, sfx.Config.Position.y, 0.0f });
        }
    }

    void AudioSubsystem::CleanupStoppedSounds()
    {
        // Remove stopped SFX
        ActiveSFX.erase(
            std::remove_if(ActiveSFX.begin(), ActiveSFX.end(),
                [](const SFXInstance& inst) {
                    return !inst.Sound || inst.Sound->getStatus() == sound::Status::Stopped;
                }),
            ActiveSFX.end()
        );

        // Remove stopped voice
        ActiveVoice.erase(
            std::remove_if(ActiveVoice.begin(), ActiveVoice.end(),
                [](const SFXInstance& inst) {
                    return !inst.Sound || inst.Sound->getStatus() == sound::Status::Stopped;
                }),
            ActiveVoice.end()
        );
    }

    void AudioSubsystem::ApplyVolumeToMusic(MusicTrack& Track)
    {
        if (!Track.Music)
            return;

        Volume effective = Track.CurrentVolume;
        
        // Apply channel volume
        AudioChannel channel = (&Track == CurrentMusic.get()) ? AudioChannel::Music : AudioChannel::Ambient;
        effective *= GetEffectiveChannelVolume(channel);

        Track.Music->setVolume(effective * 100.0f);  // SFML uses 0-100
    }

    void AudioSubsystem::ApplyVolumeToSFX(SFXInstance& Instance)
    {
        if (!Instance.Sound)
            return;

        Volume effective = Instance.CurrentVolume * GetEffectiveChannelVolume(AudioChannel::SFX);
        Instance.Sound->setVolume(effective * 100.0f);
    }

    Volume AudioSubsystem::GetEffectiveChannelVolume(AudioChannel Channel) const
    {
        if (bMasterMuted || ChannelMuted[static_cast<size_t>(Channel)])
            return 0.0f;

        return MasterVolume * ChannelVolumes[static_cast<size_t>(Channel)];
    }
}
