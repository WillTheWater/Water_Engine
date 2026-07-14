// =============================================================================
// Water Engine v2.2.4
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/AudioSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Utility/Math.h"
#include "Utility/Log.h"

namespace we
{
    AudioSubsystem* AudioSubsystem::Instance = nullptr;

    AudioSubsystem::AudioSubsystem()
    {
        Instance = this;
        ChannelVolumes.fill(0.5f);  // Default 50% volume for all channels
        ChannelMuted.fill(false);
    }

    AudioSubsystem& AudioSubsystem::Get()
    {
        return *Instance;
    }

    void AudioSubsystem::Update(float DeltaTime)
    {
        UpdateFades(DeltaTime);
        CleanupStoppedSounds();
    }

    void AudioSubsystem::PlayMusic(const string& Path, float FadeInDuration)
    {
        auto MusicResource = LoadAsset().LoadMusic(Path);
        if (!MusicResource)
        {
            ERROR("[Audio] Failed to load music: {}", Path);
            return;
        }

        CurrentMusic = make_unique<MusicTrack>();
        CurrentMusic->Music = MusicResource;
        CurrentMusic->Music->setLooping(true);
        // Head-locked: the listener moves through the world, so every
        // non-positional source must ride it or it would attenuate.
        CurrentMusic->Music->setRelativeToListener(true);

        if (FadeInDuration > 0.0f)
        {
            CurrentMusic->CurrentVolume = 0.0f;
            CurrentMusic->TargetVolume = 1.0f;
            CurrentMusic->FadeSpeed = 1.0f / FadeInDuration;
            CurrentMusic->bFadingIn = true;
        }
        else
        {
            CurrentMusic->CurrentVolume = 1.0f;
            CurrentMusic->FadeSpeed = 0.0f;
        }

        ApplyVolumeToMusic(*CurrentMusic, AudioChannel::Music);
        
        if (!bPaused)
        {
            CurrentMusic->Music->play();
        }
    }

    void AudioSubsystem::PlayAmbient(const string& Path, float FadeInDuration)
    {
        auto MusicResource = LoadAsset().LoadMusic(Path);
        if (!MusicResource)
        {
            ERROR("[Audio] Failed to load ambient: {}", Path);
            return;
        }

        CurrentAmbient = make_unique<MusicTrack>();
        CurrentAmbient->Music = MusicResource;
        CurrentAmbient->Music->setLooping(true);
        CurrentAmbient->Music->setRelativeToListener(true);

        if (FadeInDuration > 0.0f)
        {
            CurrentAmbient->CurrentVolume = 0.0f;
            CurrentAmbient->TargetVolume = 1.0f;
            CurrentAmbient->FadeSpeed = 1.0f / FadeInDuration;
            CurrentAmbient->bFadingIn = true;
        }
        else
        {
            CurrentAmbient->CurrentVolume = 1.0f;
            CurrentAmbient->FadeSpeed = 0.0f;
        }

        ApplyVolumeToMusic(*CurrentAmbient, AudioChannel::Ambient);
        
        if (!bPaused)
        {
            CurrentAmbient->Music->play();
        }
    }

    void AudioSubsystem::PlaySFX(const string& Path, Volume Gain)
    {
        auto Buffer = LoadAsset().LoadSound(Path);
        if (!Buffer)
        {
            ERROR("[Audio] Failed to load SFX: {}", Path);
            return;
        }

        MakeRoomForSFX();

        SFXInstance Instance;
        Instance.Buffer = Buffer;
        Instance.Sound = make_unique<sound>(*Buffer);
        Instance.UserVolume = Clamp(Gain, 0.0f, 1.0f);
        Instance.Sound->setRelativeToListener(true);
        ApplyOneShotVolume(Instance, AudioChannel::SFX);

        if (!bPaused)
        {
            Instance.Sound->play();
        }

        ActiveSFX.push_back(std::move(Instance));
    }

    void AudioSubsystem::PlayUI(const string& Path, Volume Gain)
    {
        auto Buffer = LoadAsset().LoadSound(Path);
        if (!Buffer)
        {
            ERROR("[Audio] Failed to load UI sound: {}", Path);
            return;
        }

        SFXInstance Instance;
        Instance.Buffer = Buffer;
        Instance.Sound = make_unique<sound>(*Buffer);
        Instance.UserVolume = Clamp(Gain, 0.0f, 1.0f);
        Instance.Sound->setRelativeToListener(true);
        ApplyOneShotVolume(Instance, AudioChannel::UI);
        Instance.Sound->play();

        ActiveUI.push_back(std::move(Instance));
    }

    void AudioSubsystem::SetListenerPosition(vec2f WorldPos)
    {
        sf::Listener::setPosition({WorldPos.x, WorldPos.y, 0.0f});
    }

    void AudioSubsystem::PlaySFX(const string& Path, vec2f WorldPos, float MinDistance, float Attenuation,
                                 Volume Gain)
    {
        auto Buffer = LoadAsset().LoadSound(Path);
        if (!Buffer)
        {
            ERROR("[Audio] Failed to load SFX: {}", Path);
            return;
        }

        MakeRoomForSFX();

        SFXInstance Instance;
        Instance.Buffer = Buffer;
        Instance.Sound = make_unique<sound>(*Buffer);
        Instance.UserVolume = Clamp(Gain, 0.0f, 1.0f);
        Instance.Sound->setRelativeToListener(false);
        Instance.Sound->setPosition({WorldPos.x, WorldPos.y, 0.0f});
        Instance.Sound->setMinDistance(MinDistance);
        Instance.Sound->setAttenuation(Attenuation);
        ApplyOneShotVolume(Instance, AudioChannel::SFX);

        if (!bPaused)
        {
            Instance.Sound->play();
        }

        ActiveSFX.push_back(std::move(Instance));
    }

    AudioLoopId AudioSubsystem::PlayLoop(const string& Path, vec2f WorldPos, float MinDistance, float Attenuation)
    {
        auto Buffer = LoadAsset().LoadSound(Path);
        if (!Buffer)
        {
            ERROR("[Audio] Failed to load loop: {}", Path);
            return 0;
        }

        LoopInstance Instance;
        Instance.Id = NextLoopId++;
        Instance.Buffer = Buffer;
        Instance.Sound = make_unique<sound>(*Buffer);
        Instance.Sound->setLooping(true);
        Instance.Sound->setRelativeToListener(false);
        Instance.Sound->setPosition({WorldPos.x, WorldPos.y, 0.0f});
        Instance.Sound->setMinDistance(MinDistance);
        Instance.Sound->setAttenuation(Attenuation);
        ApplyLoopVolume(Instance);

        if (!bPaused)
        {
            Instance.Sound->play();
        }

        AudioLoopId Id = Instance.Id;
        ActiveLoops.push_back(std::move(Instance));
        return Id;
    }

    AudioLoopId AudioSubsystem::PlayLoop(const string& Path)
    {
        auto Buffer = LoadAsset().LoadSound(Path);
        if (!Buffer)
        {
            ERROR("[Audio] Failed to load loop: {}", Path);
            return 0;
        }

        LoopInstance Instance;
        Instance.Id = NextLoopId++;
        Instance.Buffer = Buffer;
        Instance.Sound = make_unique<sound>(*Buffer);
        Instance.Sound->setLooping(true);
        Instance.Sound->setRelativeToListener(true);
        ApplyLoopVolume(Instance);

        if (!bPaused)
        {
            Instance.Sound->play();
        }

        AudioLoopId Id = Instance.Id;
        ActiveLoops.push_back(std::move(Instance));
        return Id;
    }

    void AudioSubsystem::SetLoopPosition(AudioLoopId Id, vec2f WorldPos)
    {
        if (LoopInstance* Loop = FindLoop(Id))
        {
            Loop->Sound->setPosition({WorldPos.x, WorldPos.y, 0.0f});
        }
    }

    void AudioSubsystem::SetLoopVolume(AudioLoopId Id, Volume Vol)
    {
        if (LoopInstance* Loop = FindLoop(Id))
        {
            Loop->UserVolume = Clamp(Vol, 0.0f, 1.0f);
            ApplyLoopVolume(*Loop);
        }
    }

    void AudioSubsystem::StopLoop(AudioLoopId Id)
    {
        for (auto it = ActiveLoops.begin(); it != ActiveLoops.end(); ++it)
        {
            if (it->Id == Id)
            {
                if (it->Sound)
                {
                    it->Sound->stop();
                }
                ActiveLoops.erase(it);
                return;
            }
        }
    }

    void AudioSubsystem::StopAllLoops()
    {
        for (auto& Loop : ActiveLoops)
        {
            if (Loop.Sound)
            {
                Loop.Sound->stop();
            }
        }
        ActiveLoops.clear();
    }

    void AudioSubsystem::PlayVoice(const string& Path, Volume Gain)
    {
        auto Buffer = LoadAsset().LoadSound(Path);
        if (!Buffer)
        {
            ERROR("[Audio] Failed to load voice: {}", Path);
            return;
        }

        SFXInstance Instance;
        Instance.Buffer = Buffer;
        Instance.Sound = make_unique<sound>(*Buffer);
        Instance.UserVolume = Clamp(Gain, 0.0f, 1.0f);
        Instance.Sound->setRelativeToListener(true);
        ApplyOneShotVolume(Instance, AudioChannel::Voice);

        if (!bPaused)
        {
            Instance.Sound->play();
        }

        ActiveVoice.push_back(std::move(Instance));
    }

    void AudioSubsystem::StopMusic(float FadeDuration)
    {
        if (!CurrentMusic || !CurrentMusic->Music)
            return;

        if (FadeDuration > 0.0f)
        {
            CurrentMusic->TargetVolume = 0.0f;
            CurrentMusic->FadeSpeed = CurrentMusic->CurrentVolume / FadeDuration;
        }
        else
        {
            CurrentMusic->Music->stop();
            CurrentMusic.reset();
        }
    }

    void AudioSubsystem::StopAmbient(float FadeDuration)
    {
        if (!CurrentAmbient || !CurrentAmbient->Music)
            return;

        if (FadeDuration > 0.0f)
        {
            CurrentAmbient->TargetVolume = 0.0f;
            CurrentAmbient->FadeSpeed = CurrentAmbient->CurrentVolume / FadeDuration;
        }
        else
        {
            CurrentAmbient->Music->stop();
            CurrentAmbient.reset();
        }
    }

    void AudioSubsystem::CrossfadeMusic(const string& Path, float Duration)
    {
        if (Duration <= 0.0f)
        {
            PlayMusic(Path);
            return;
        }

        // Move current music to fading slot and fade it out
        if (CurrentMusic && CurrentMusic->Music)
        {
            FadingMusic = std::move(CurrentMusic);
            FadingMusic->TargetVolume = 0.0f;
            FadingMusic->FadeSpeed = FadingMusic->CurrentVolume / Duration;
            FadingMusic->bFadingIn = false;
        }

        // Load and start new music with fade in
        auto MusicResource = LoadAsset().LoadMusic(Path);
        if (!MusicResource)
        {
            ERROR("[Audio] Failed to load music for crossfade: {}", Path);
            return;
        }

        CurrentMusic = make_unique<MusicTrack>();
        CurrentMusic->Music = MusicResource;
        CurrentMusic->Music->setLooping(true);
        CurrentMusic->CurrentVolume = 0.0f;
        CurrentMusic->TargetVolume = 1.0f;
        CurrentMusic->FadeSpeed = 1.0f / Duration;
        CurrentMusic->bFadingIn = true;

        ApplyVolumeToMusic(*CurrentMusic, AudioChannel::Music);
        
        if (!bPaused)
        {
            CurrentMusic->Music->play();
        }
    }

    void AudioSubsystem::CrossfadeAmbient(const string& Path, float Duration)
    {
        if (Duration <= 0.0f)
        {
            PlayAmbient(Path);
            return;
        }

        // Move current ambient to fading slot and fade it out
        if (CurrentAmbient && CurrentAmbient->Music)
        {
            FadingAmbient = std::move(CurrentAmbient);
            FadingAmbient->TargetVolume = 0.0f;
            FadingAmbient->FadeSpeed = FadingAmbient->CurrentVolume / Duration;
            FadingAmbient->bFadingIn = false;
        }

        // Load and start new ambient with fade in
        auto MusicResource = LoadAsset().LoadMusic(Path);
        if (!MusicResource)
        {
            ERROR("[Audio] Failed to load ambient for crossfade: {}", Path);
            return;
        }

        CurrentAmbient = make_unique<MusicTrack>();
        CurrentAmbient->Music = MusicResource;
        CurrentAmbient->Music->setLooping(true);
        CurrentAmbient->Music->setRelativeToListener(true);
        CurrentAmbient->CurrentVolume = 0.0f;
        CurrentAmbient->TargetVolume = 1.0f;
        CurrentAmbient->FadeSpeed = 1.0f / Duration;
        CurrentAmbient->bFadingIn = true;

        ApplyVolumeToMusic(*CurrentAmbient, AudioChannel::Ambient);
        
        if (!bPaused)
        {
            CurrentAmbient->Music->play();
        }
    }

    void AudioSubsystem::StopAllSFX()
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

    void AudioSubsystem::StopAllVoice()
    {
        for (auto& Instance : ActiveVoice)
        {
            if (Instance.Sound)
            {
                Instance.Sound->stop();
            }
        }
        ActiveVoice.clear();
    }

    void AudioSubsystem::StopAllUI()
    {
        for (auto& Instance : ActiveUI)
        {
            if (Instance.Sound)
            {
                Instance.Sound->stop();
            }
        }
        ActiveUI.clear();
    }

    void AudioSubsystem::StopAll()
    {
        StopMusic();
        StopAmbient();
        StopAllSFX();
        StopAllVoice();
        StopAllUI();
        StopAllLoops();
        
        // Also stop fading tracks
        if (FadingMusic && FadingMusic->Music)
        {
            FadingMusic->Music->stop();
            FadingMusic.reset();
        }
        if (FadingAmbient && FadingAmbient->Music)
        {
            FadingAmbient->Music->stop();
            FadingAmbient.reset();
        }
    }

    void AudioSubsystem::SetMasterVolume(Volume Vol)
    {
        MasterVolume = Clamp(Vol, 0.0f, 1.0f);
        
        if (CurrentMusic && CurrentMusic->Music)
        {
            ApplyVolumeToMusic(*CurrentMusic, AudioChannel::Music);
        }
        if (FadingMusic && FadingMusic->Music)
        {
            ApplyVolumeToMusic(*FadingMusic, AudioChannel::Music);
        }
        if (CurrentAmbient && CurrentAmbient->Music)
        {
            ApplyVolumeToMusic(*CurrentAmbient, AudioChannel::Ambient);
        }
        if (FadingAmbient && FadingAmbient->Music)
        {
            ApplyVolumeToMusic(*FadingAmbient, AudioChannel::Ambient);
        }
        for (auto& Instance : ActiveSFX)
        {
            ApplyOneShotVolume(Instance, AudioChannel::SFX);
        }
        for (auto& Instance : ActiveVoice)
        {
            ApplyOneShotVolume(Instance, AudioChannel::Voice);
        }
        for (auto& Instance : ActiveUI)
        {
            ApplyOneShotVolume(Instance, AudioChannel::UI);
        }
        for (auto& Loop : ActiveLoops)
        {
            ApplyLoopVolume(Loop);
        }
    }

    Volume AudioSubsystem::GetMasterVolume() const
    {
        return MasterVolume;
    }

    void AudioSubsystem::SetChannelVolume(AudioChannel Channel, Volume Vol)
    {
        if (Channel == AudioChannel::Master)
        {
            SetMasterVolume(Vol);
            return;
        }

        auto Index = static_cast<ulong>(Channel);
        ChannelVolumes[Index] = Clamp(Vol, 0.0f, 1.0f);

        if (Channel == AudioChannel::Music)
        {
            if (CurrentMusic && CurrentMusic->Music)
                ApplyVolumeToMusic(*CurrentMusic, Channel);
            if (FadingMusic && FadingMusic->Music)
                ApplyVolumeToMusic(*FadingMusic, Channel);
        }
        else if (Channel == AudioChannel::Ambient)
        {
            if (CurrentAmbient && CurrentAmbient->Music)
                ApplyVolumeToMusic(*CurrentAmbient, Channel);
            if (FadingAmbient && FadingAmbient->Music)
                ApplyVolumeToMusic(*FadingAmbient, Channel);
            for (auto& Loop : ActiveLoops)
            {
                ApplyLoopVolume(Loop);
            }
        }
        else if (Channel == AudioChannel::SFX)
        {
            for (auto& Instance : ActiveSFX)
            {
                ApplyOneShotVolume(Instance, Channel);
            }
        }
        else if (Channel == AudioChannel::Voice)
        {
            for (auto& Instance : ActiveVoice)
            {
                ApplyOneShotVolume(Instance, Channel);
            }
        }
        else if (Channel == AudioChannel::UI)
        {
            for (auto& Instance : ActiveUI)
            {
                ApplyOneShotVolume(Instance, Channel);
            }
        }
    }

    Volume AudioSubsystem::GetChannelVolume(AudioChannel Channel) const
    {
        if (Channel == AudioChannel::Master)
            return MasterVolume;
        return ChannelVolumes[static_cast<ulong>(Channel)];
    }

    void AudioSubsystem::SetMasterMuted(bool bMuted)
    {
        bMasterMuted = bMuted;
        SetMasterVolume(MasterVolume);
    }

    bool AudioSubsystem::IsMasterMuted() const
    {
        return bMasterMuted;
    }

    void AudioSubsystem::SetChannelMuted(AudioChannel Channel, bool bMuted)
    {
        if (Channel == AudioChannel::Master)
        {
            SetMasterMuted(bMuted);
            return;
        }

        ChannelMuted[static_cast<ulong>(Channel)] = bMuted;
        SetChannelVolume(Channel, ChannelVolumes[static_cast<ulong>(Channel)]);
    }

    bool AudioSubsystem::IsChannelMuted(AudioChannel Channel) const
    {
        if (Channel == AudioChannel::Master)
            return bMasterMuted;
        return ChannelMuted[static_cast<ulong>(Channel)];
    }

    void AudioSubsystem::SetPaused(bool bInPaused)
    {
        if (bPaused == bInPaused)
            return;

        bPaused = bInPaused;

        // Music (current)
        if (CurrentMusic && CurrentMusic->Music)
        {
            if (bPaused) CurrentMusic->Music->pause();
            else CurrentMusic->Music->play();
        }

        // Music (fading)
        if (FadingMusic && FadingMusic->Music)
        {
            if (bPaused) FadingMusic->Music->pause();
            else FadingMusic->Music->play();
        }

        // Ambient (current)
        if (CurrentAmbient && CurrentAmbient->Music)
        {
            if (bPaused) CurrentAmbient->Music->pause();
            else CurrentAmbient->Music->play();
        }

        // Ambient (fading)
        if (FadingAmbient && FadingAmbient->Music)
        {
            if (bPaused) FadingAmbient->Music->pause();
            else FadingAmbient->Music->play();
        }

        // SFX
        for (auto& Instance : ActiveSFX)
        {
            if (Instance.Sound)
            {
                if (bPaused) Instance.Sound->pause();
                else Instance.Sound->play();
            }
        }

        // Voice
        for (auto& Instance : ActiveVoice)
        {
            if (Instance.Sound)
            {
                if (bPaused) Instance.Sound->pause();
                else Instance.Sound->play();
            }
        }

        // Positional loops
        for (auto& Loop : ActiveLoops)
        {
            if (Loop.Sound)
            {
                if (bPaused) Loop.Sound->pause();
                else Loop.Sound->play();
            }
        }

        // ActiveUI is deliberately untouched: menu clicks play while paused.
    }

    bool AudioSubsystem::IsPaused() const
    {
        return bPaused;
    }

    bool AudioSubsystem::IsMusicPlaying() const
    {
        bool CurrentPlaying = CurrentMusic && CurrentMusic->Music && 
               CurrentMusic->Music->getStatus() == sf::SoundSource::Status::Playing;
        bool FadingPlaying = FadingMusic && FadingMusic->Music && 
               FadingMusic->Music->getStatus() == sf::SoundSource::Status::Playing;
        return CurrentPlaying || FadingPlaying;
    }

    bool AudioSubsystem::IsAmbientPlaying() const
    {
        bool CurrentPlaying = CurrentAmbient && CurrentAmbient->Music && 
               CurrentAmbient->Music->getStatus() == sf::SoundSource::Status::Playing;
        bool FadingPlaying = FadingAmbient && FadingAmbient->Music && 
               FadingAmbient->Music->getStatus() == sf::SoundSource::Status::Playing;
        return CurrentPlaying || FadingPlaying;
    }

    ulong AudioSubsystem::GetActiveSFXCount() const
    {
        return ActiveSFX.size();
    }

    ulong AudioSubsystem::GetActiveVoiceCount() const
    {
        return ActiveVoice.size();
    }

    ulong AudioSubsystem::GetActiveUICount() const
    {
        return ActiveUI.size();
    }

    ulong AudioSubsystem::GetActiveLoopCount() const
    {
        return ActiveLoops.size();
    }

    AudioSubsystem::LoopInstance* AudioSubsystem::FindLoop(AudioLoopId Id)
    {
        for (auto& Loop : ActiveLoops)
        {
            if (Loop.Id == Id)
                return &Loop;
        }
        return nullptr;
    }

    void AudioSubsystem::ApplyLoopVolume(LoopInstance& Loop)
    {
        if (!Loop.Sound) return;
        Loop.Sound->setVolume(GetEffectiveVolume(AudioChannel::Ambient) * Loop.UserVolume * 100.0f);
    }

    void AudioSubsystem::ApplyOneShotVolume(SFXInstance& Instance, AudioChannel Channel)
    {
        if (!Instance.Sound) return;
        Instance.Sound->setVolume(GetEffectiveVolume(Channel) * Instance.UserVolume * 100.0f);
    }

    void AudioSubsystem::MakeRoomForSFX()
    {
        if (ActiveSFX.size() < MAX_ACTIVE_SFX)
            return;

        if (ActiveSFX.front().Sound)
            ActiveSFX.front().Sound->stop();
        ActiveSFX.erase(ActiveSFX.begin());
    }

    void AudioSubsystem::UpdateFades(float DeltaTime)
    {
        // Music fade (current)
        if (CurrentMusic && CurrentMusic->FadeSpeed > 0.0f)
        {
            if (CurrentMusic->bFadingIn)
            {
                // Fade in: volume increases to target
                CurrentMusic->CurrentVolume += CurrentMusic->FadeSpeed * DeltaTime;
                if (CurrentMusic->CurrentVolume >= CurrentMusic->TargetVolume)
                {
                    CurrentMusic->CurrentVolume = CurrentMusic->TargetVolume;
                    CurrentMusic->FadeSpeed = 0.0f;
                    CurrentMusic->bFadingIn = false;
                }
                ApplyVolumeToMusic(*CurrentMusic, AudioChannel::Music);
            }
            else if (CurrentMusic->CurrentVolume > CurrentMusic->TargetVolume)
            {
                // Fade out: volume decreases to target
                CurrentMusic->CurrentVolume -= CurrentMusic->FadeSpeed * DeltaTime;
                if (CurrentMusic->CurrentVolume <= CurrentMusic->TargetVolume)
                {
                    CurrentMusic->CurrentVolume = CurrentMusic->TargetVolume;
                    CurrentMusic->Music->stop();
                    CurrentMusic.reset();
                }
                else
                {
                    ApplyVolumeToMusic(*CurrentMusic, AudioChannel::Music);
                }
            }
        }

        // Music fade (fading out during crossfade)
        if (FadingMusic && FadingMusic->FadeSpeed > 0.0f)
        {
            // Always fading out
            FadingMusic->CurrentVolume -= FadingMusic->FadeSpeed * DeltaTime;
            if (FadingMusic->CurrentVolume <= 0.0f)
            {
                FadingMusic->CurrentVolume = 0.0f;
                FadingMusic->Music->stop();
                FadingMusic.reset();
            }
            else
            {
                ApplyVolumeToMusic(*FadingMusic, AudioChannel::Music);
            }
        }

        // Ambient fade (current)
        if (CurrentAmbient && CurrentAmbient->FadeSpeed > 0.0f)
        {
            if (CurrentAmbient->bFadingIn)
            {
                // Fade in: volume increases to target
                CurrentAmbient->CurrentVolume += CurrentAmbient->FadeSpeed * DeltaTime;
                if (CurrentAmbient->CurrentVolume >= CurrentAmbient->TargetVolume)
                {
                    CurrentAmbient->CurrentVolume = CurrentAmbient->TargetVolume;
                    CurrentAmbient->FadeSpeed = 0.0f;
                    CurrentAmbient->bFadingIn = false;
                }
                ApplyVolumeToMusic(*CurrentAmbient, AudioChannel::Ambient);
            }
            else if (CurrentAmbient->CurrentVolume > CurrentAmbient->TargetVolume)
            {
                // Fade out: volume decreases to target
                CurrentAmbient->CurrentVolume -= CurrentAmbient->FadeSpeed * DeltaTime;
                if (CurrentAmbient->CurrentVolume <= CurrentAmbient->TargetVolume)
                {
                    CurrentAmbient->CurrentVolume = CurrentAmbient->TargetVolume;
                    CurrentAmbient->Music->stop();
                    CurrentAmbient.reset();
                }
                else
                {
                    ApplyVolumeToMusic(*CurrentAmbient, AudioChannel::Ambient);
                }
            }
        }

        // Ambient fade (fading out during crossfade)
        if (FadingAmbient && FadingAmbient->FadeSpeed > 0.0f)
        {
            // Always fading out
            FadingAmbient->CurrentVolume -= FadingAmbient->FadeSpeed * DeltaTime;
            if (FadingAmbient->CurrentVolume <= 0.0f)
            {
                FadingAmbient->CurrentVolume = 0.0f;
                FadingAmbient->Music->stop();
                FadingAmbient.reset();
            }
            else
            {
                ApplyVolumeToMusic(*FadingAmbient, AudioChannel::Ambient);
            }
        }
    }

    void AudioSubsystem::CleanupStoppedSounds()
    {
        // Remove finished SFX
        for (auto it = ActiveSFX.begin(); it != ActiveSFX.end();)
        {
            if (it->Sound && it->Sound->getStatus() == sf::SoundSource::Status::Stopped)
            {
                it = ActiveSFX.erase(it);
            }
            else
            {
                ++it;
            }
        }

        // Remove finished Voice
        for (auto it = ActiveVoice.begin(); it != ActiveVoice.end();)
        {
            if (it->Sound && it->Sound->getStatus() == sf::SoundSource::Status::Stopped)
            {
                it = ActiveVoice.erase(it);
            }
            else
            {
                ++it;
            }
        }

        // Remove finished UI
        for (auto it = ActiveUI.begin(); it != ActiveUI.end();)
        {
            if (it->Sound && it->Sound->getStatus() == sf::SoundSource::Status::Stopped)
            {
                it = ActiveUI.erase(it);
            }
            else
            {
                ++it;
            }
        }

        // Clean up finished music that wasn't faded
        if (CurrentMusic && CurrentMusic->Music && 
            CurrentMusic->Music->getStatus() == sf::SoundSource::Status::Stopped &&
            CurrentMusic->FadeSpeed == 0.0f)
        {
            CurrentMusic.reset();
        }

        if (FadingMusic && FadingMusic->Music && 
            FadingMusic->Music->getStatus() == sf::SoundSource::Status::Stopped)
        {
            FadingMusic.reset();
        }

        if (CurrentAmbient && CurrentAmbient->Music && 
            CurrentAmbient->Music->getStatus() == sf::SoundSource::Status::Stopped &&
            CurrentAmbient->FadeSpeed == 0.0f)
        {
            CurrentAmbient.reset();
        }

        if (FadingAmbient && FadingAmbient->Music && 
            FadingAmbient->Music->getStatus() == sf::SoundSource::Status::Stopped)
        {
            FadingAmbient.reset();
        }
    }

    void AudioSubsystem::ApplyVolumeToMusic(MusicTrack& Track, AudioChannel Channel)
    {
        if (!Track.Music)
            return;

        Volume EffectiveVol = GetEffectiveVolume(Channel) * Track.CurrentVolume;
        Track.Music->setVolume(EffectiveVol * 100.0f);
    }

    Volume AudioSubsystem::GetEffectiveVolume(AudioChannel Channel) const
    {
        if (Channel == AudioChannel::Master)
            return bMasterMuted ? 0.0f : MasterVolume;

        auto Index = static_cast<ulong>(Channel);
        Volume ChannelVol = ChannelMuted[Index] ? 0.0f : ChannelVolumes[Index];
        Volume MasterVol = bMasterMuted ? 0.0f : MasterVolume;
        
        return MasterVol * ChannelVol;
    }
}